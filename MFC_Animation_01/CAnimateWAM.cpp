#include "pch.h"
#include "CAnimateWAM.h"

#include <dwmapi.h>
#include <math.h>


CAnimateWAM::CAnimateWAM(int sizeWindow, float spacingTile) : _sizeWidow(sizeWindow), _spacingTile(spacingTile)
{

}

void CAnimateWAM::SetHwndSizeTileSpacing(HWND hWnd, int sizeWindow, float spacingTile)
{
    _hwnd = hWnd;
    if (sizeWindow > 0) {
        _sizeWidow = sizeWindow;
        _spacingTile = spacingTile;
    }
}

CAnimateWAM::~CAnimateWAM()
{
    AfterLeavingMessageLoop();
}


//----------------------------------------------------------
// Create an instance of WAM Manager Object which manages storyboards, 
// transitions, and variables
//----------------------------------------------------------
HRESULT CAnimateWAM::CreateAnimationManager()
{
    return ::CoCreateInstance(
        CLSID_UIAnimationManager2,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IUIAnimationManager2,
        reinterpret_cast<LPVOID*>(&_manager));
}

//-----------------------------------------------------------
// Creates an WAM transition library which enables us to schedule 
// transitions
//-----------------------------------------------------------
HRESULT CAnimateWAM::CreateAnimationTransitionLibrary()
{
    return ::CoCreateInstance(
        CLSID_UIAnimationTransitionLibrary2,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_IUIAnimationTransitionLibrary2,
        reinterpret_cast<LPVOID*>(&_transitionLibrary));
}

//-----------------------------------------------------------
// Creates an WAM animation variable  which we will use to animate 
// the tiles
//-----------------------------------------------------------
HRESULT CAnimateWAM::CreateAnimationVariables()
{
    HRESULT hr = (_manager == nullptr) ? E_UNEXPECTED : S_OK;
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr))
    {
        hr = _manager->CreateAnimationVariable(0.0, &_animationVariable);
        assert(SUCCEEDED(hr));
    }

    return hr;
}

HRESULT CAnimateWAM::CreateD3D11Device()
{
    HRESULT hr = S_OK;

    D3D_DRIVER_TYPE driverTypes[] =
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
    };

    D3D_FEATURE_LEVEL featureLevelSupported;

    for (int i = 0; i < sizeof(driverTypes) / sizeof(driverTypes[0]); ++i)
    {
        CComPtr<ID3D11Device> d3d11Device;
        CComPtr<ID3D11DeviceContext> d3d11DeviceContext;

        hr = D3D11CreateDevice(
            nullptr,
            driverTypes[i],
            NULL,
            D3D11_CREATE_DEVICE_BGRA_SUPPORT,
            NULL,
            0,
            D3D11_SDK_VERSION,
            &d3d11Device,
            &featureLevelSupported,
            &d3d11DeviceContext);
        assert(SUCCEEDED(hr));

        if (SUCCEEDED(hr))
        {
            _d3d11Device = d3d11Device.Detach();
            _d3d11DeviceContext = d3d11DeviceContext.Detach();

            break;
        }
    }

    return hr;
}

HRESULT CAnimateWAM::CreateD2D1Factory()
{
    return D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &_d2d1Factory);
}

HRESULT CAnimateWAM::CreateD2D1Device()
{
    HRESULT hr = ((_d3d11Device == nullptr) || (_d2d1Factory == nullptr)) ? E_UNEXPECTED : S_OK;

    CComPtr<IDXGIDevice> dxgiDevice;

    if (SUCCEEDED(hr))
    {
        hr = _d3d11Device->QueryInterface(&dxgiDevice);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = _d2d1Factory->CreateDevice(dxgiDevice, &_d2d1Device);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = _d2d1Device->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, &_d2d1DeviceContext);
        assert(SUCCEEDED(hr));
    }

    return hr;
}

HRESULT CAnimateWAM::CreateWICFactory()
{
    return CoCreateInstance(
        CLSID_WICImagingFactory,
        nullptr,
        CLSCTX_INPROC_SERVER,
        IID_PPV_ARGS(&_wicFactory));
}

void CAnimateWAM::DestroyAnimationVariables()
{
    _animationVariable = nullptr;
}

void CAnimateWAM::DestroyAnimationTransitionLibrary()
{
    _transitionLibrary = nullptr;
}

void CAnimateWAM::DestroyAnimationManager()
{
    _manager = nullptr;
}

void CAnimateWAM::DestroyWICFactory()
{
    _wicFactory = nullptr;
}

void CAnimateWAM::DestroyD2D1Device()
{
    _d2d1DeviceContext = nullptr;
    _d2d1Device = nullptr;
}

void CAnimateWAM::DestroyD2D1Factory()
{
    _d2d1Factory = nullptr;
}

void CAnimateWAM::DestroyD3D11Device()
{
    _d3d11DeviceContext = nullptr;
    _d3d11Device = nullptr;
}


//-----------------------------------------------------------
// Creates a DirectComposition device
//-----------------------------------------------------------
HRESULT CAnimateWAM::CreateDCompositionDevice()
{
    HRESULT hr = (_d3d11Device == nullptr) ? E_UNEXPECTED : S_OK;
    assert(SUCCEEDED(hr));

    CComPtr<IDXGIDevice> dxgiDevice;

    if (SUCCEEDED(hr))
    {
        hr = _d3d11Device->QueryInterface(&dxgiDevice);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = DCompositionCreateDevice(dxgiDevice, __uuidof(IDCompositionDevice), reinterpret_cast<void**>(&_device));
        assert(SUCCEEDED(hr));
    }

    return hr;
}

//--------------------------------------------------------
// Creates an render target for DirectComposition which
// is an hwnd in this case
//--------------------------------------------------------
HRESULT CAnimateWAM::CreateDCompositionRenderTarget()
{
    HRESULT hr = ((_device == nullptr) || (_hwnd == NULL)) ? E_UNEXPECTED : S_OK;
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr))
    {
        hr = _device->CreateTargetForHwnd(_hwnd, TRUE, &_target);
        assert(SUCCEEDED(hr));
    }

    return hr;
}

//---------------------------------------------------------------------
// Creates a DirectComposition visual tree and places each visual 
// inside the application window
//---------------------------------------------------------------------
HRESULT CAnimateWAM::CreateDCompositionVisualTreeFromFile(const WCHAR* filename)
{
    static const float tileSize = 0.3f * _sizeWidow;
    static const int visualChildCount = sizeof(_visualChild) / sizeof(_visualChild[0]);
    static const float d = 2.0f * _sizeWidow;

    HRESULT hr = ((_device == nullptr) || (_hwnd == NULL)) ? E_UNEXPECTED : S_OK;
    assert(SUCCEEDED(hr));

    int bitmapWidth = 0;
    int bitmapHeight = 0;

    CComPtr<IDCompositionSurface> surface;

    // Create DirectComposition surface from the bitmap file
    if (SUCCEEDED(hr))
    {
        hr = CreateSurfaceFromFile(filename, &bitmapWidth, &bitmapHeight, &surface);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        _bitmapWidth = bitmapWidth;
        _bitmapHeight = bitmapHeight;

        hr = _device->CreateVisual(&_visual);
        assert(SUCCEEDED(hr));
    }

    // Set the content of each visual to be the surface that was created from the bitmap
    if (SUCCEEDED(hr))
    {
        for (int i = 0; SUCCEEDED(hr) && i < visualChildCount; ++i)
        {
            hr = _device->CreateVisual(&_visualChild[i]);

            if (SUCCEEDED(hr))
            {
                hr = _visual->AddVisual(_visualChild[i], FALSE, nullptr);
            }

            if (SUCCEEDED(hr))
            {
                hr = _visualChild[i]->SetContent(surface);
            }
        }
    }

    // Using DirectComposition transforms to scale and place each visual such that the tiles 
    // are side by side within the application window
    if (SUCCEEDED(hr))
    {
        for (int i = 0; SUCCEEDED(hr) && i < visualChildCount; ++i)
        {
            //setting up scale transform on each visual
            CComPtr<IDCompositionScaleTransform> scaleTransform;

            if (SUCCEEDED(hr))
            {
                hr = _device->CreateScaleTransform(&scaleTransform);
            }

            float sx = tileSize / bitmapWidth;

            if (SUCCEEDED(hr))
            {
                hr = scaleTransform->SetScaleX(sx);
            }

            float sy = tileSize / bitmapHeight;

            if (SUCCEEDED(hr))
            {
                hr = scaleTransform->SetScaleY(sy);
            }

            //Setting up a translate transform on each visual
            CComPtr<IDCompositionTranslateTransform> translateTransform;

            if (SUCCEEDED(hr))
            {
                hr = _device->CreateTranslateTransform(&translateTransform);
            }

            float x = (visualChildCount - 1 - i) * _spacingTile;
            float y = _spacingTile + 30;

            if (SUCCEEDED(hr))
            {
                hr = translateTransform->SetOffsetX(x);
            }

            if (SUCCEEDED(hr))
            {
                hr = translateTransform->SetOffsetY(y);
            }

            // Creating a transform group to group the two transforms together such that 
            // they can be applied at once.
            IDCompositionTransform* transforms[] =
            {
                scaleTransform,
                translateTransform,
            };

            CComPtr<IDCompositionTransform> transformGroup;
            if (SUCCEEDED(hr))
            {
                _device->CreateTransformGroup(transforms, sizeof(transforms) / sizeof(transforms[0]), &transformGroup);
            }
            if (SUCCEEDED(hr))
            {
                _visualChild[i]->SetTransform(transformGroup);
            }
        }
    }

    return hr;
}

//-------------------------------------------------------------------------------
// Use WAM to generate and propagate the appropriate animation curves to DirectComposition when 
// keypress is detected
//-------------------------------------------------------------------------------
HRESULT CAnimateWAM::CreateSlideAnimation(DIRECTION dir, IDCompositionAnimation** slideAnimation)
{
    HRESULT hr = (slideAnimation == nullptr) ? E_POINTER : S_OK;
    assert(SUCCEEDED(hr));

    float rightMargin = 27 * _spacingTile * -1;  //where the tiles end. Note forward direction is represented by a negative value.
    float leftMargin = 0; // where the tiles begin

    if (SUCCEEDED(hr))
    {
        *slideAnimation = nullptr;
        hr = ((_device == nullptr) || (_animationVariable == nullptr)) ? E_UNEXPECTED : S_OK;
        assert(SUCCEEDED(hr));
    }

    //WAM propagates curves to DirectComposition using the IDCompositionAnimation object
    CComPtr<IDCompositionAnimation> animation;

    if (SUCCEEDED(hr))
    {
        hr = _device->CreateAnimation(&animation);
        assert(SUCCEEDED(hr));
    }

    //Create a storyboard for the slide animation
    CComPtr<IUIAnimationStoryboard2> storyboard;

    if (SUCCEEDED(hr))
    {
        hr = _manager->CreateStoryboard(&storyboard);
        assert(SUCCEEDED(hr));
    }

    // Synchronizing WAM and DirectComposition time such that when WAM Update is called, 
    // the value reflects the DirectComposition value at the given time.
    DCOMPOSITION_FRAME_STATISTICS frameStatistics = { 0 };

    if (SUCCEEDED(hr))
    {
        hr = _device->GetFrameStatistics(&frameStatistics);
        assert(SUCCEEDED(hr));
    }

    UI_ANIMATION_SECONDS nextEstimatedFrameTime = 0.0;

    if (SUCCEEDED(hr))
    {
        nextEstimatedFrameTime = static_cast<double>(frameStatistics.nextEstimatedFrameTime.QuadPart) / static_cast<double>(frameStatistics.timeFrequency.QuadPart);
    }

    //Upating the WAM time 
    if (SUCCEEDED(hr))
    {
        hr = _manager->Update(nextEstimatedFrameTime);
        assert(SUCCEEDED(hr));
    }

    CComPtr<IUIAnimationTransition2> transition;
    double curValue = 0;    //current value of the animation variable
    int velocity = 500;     //arbitrary fix velocity for the slide animation

    if (SUCCEEDED(hr))
    {
        hr = _animationVariable->GetValue(&curValue);
        assert(SUCCEEDED(hr));

        switch (dir)
        {
        case stopForward:
        case stopBackward:
            // Stopping the animation smoothly when key is let go
            if (curValue != leftMargin && curValue != rightMargin) {
                hr = _transitionLibrary->CreateSmoothStopTransition(0.5, curValue + dir * 50, &transition);
                assert(SUCCEEDED(hr));
            }
            break;
        case forward:
            // slide the tiles forward using a linear curve upon left button press
            hr = _transitionLibrary->CreateLinearTransition(-1 * (rightMargin - curValue) / velocity, rightMargin, &transition);
            assert(SUCCEEDED(hr));
            break;
        case backward:
            // slide the tiles backward using a linear cruve upon right button press
            hr = _transitionLibrary->CreateLinearTransition(-1 * curValue / velocity, leftMargin, &transition);
            assert(SUCCEEDED(hr));
            break;
        }
    }

    //Add above transition to storyboard
    if (SUCCEEDED(hr))
    {
        // allow silient failure of this function to indicate end of transition.
        hr = storyboard->AddTransition(_animationVariable, transition);
//        assert(SUCCEEDED(hr));        // allow silient failure of this function
    }

    //schedule the storyboard for play at the next estimate vblank
    if (SUCCEEDED(hr))
    {
        hr = storyboard->Schedule(nextEstimatedFrameTime);
        assert(SUCCEEDED(hr));
    }

    //Giving WAM varialbe the IDCompositionAnimation object to recieve the animation curves
    if (SUCCEEDED(hr))
    {
        hr = _animationVariable->GetCurve(animation);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        *slideAnimation = animation.Detach();
    }

    return hr;
}

HRESULT CAnimateWAM::AttachDCompositionVisualTreeToRenderTarget()
{
    HRESULT hr = ((_target == nullptr) || (_visual == nullptr)) ? E_UNEXPECTED : S_OK;
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr))
    {
        hr = _target->SetRoot(_visual);
        assert(SUCCEEDED(hr));
    }

    return hr;
}

HRESULT CAnimateWAM::DetachDCompositionVisualTreeToRenderTarget()
{
    HRESULT hr = (_target == nullptr) ? E_UNEXPECTED : S_OK;
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr))
    {
        hr = _target->SetRoot(nullptr);
        assert(SUCCEEDED(hr));
    }

    return hr;
}

void CAnimateWAM::DestroyDCompositionVisualTree()
{
    for (int i = 0; i < sizeof(_visualChild) / sizeof(_visualChild[0]); ++i)
    {
        _visualChild[i] = nullptr;
    }

    _visual = nullptr;
}

void CAnimateWAM::DestroyDCompositionRenderTarget()
{
    _target = nullptr;
}

void CAnimateWAM::DestroyDCompositionDevice()
{
    _device = nullptr;
}



HRESULT CAnimateWAM::CreateSurfaceFromFile(const WCHAR* filename, int* bitmapWidth, int* bitmapHeight, IDCompositionSurface** surface)
{
    HRESULT hr = ((bitmapWidth == nullptr) || (bitmapHeight == nullptr) || (surface == nullptr)) ? E_POINTER : S_OK;
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr))
    {
        *bitmapWidth = 0;
        *bitmapHeight = 0;
        *surface = NULL;

        hr = (filename == nullptr) ? E_INVALIDARG : S_OK;
        assert(SUCCEEDED(hr));
    }

    CComPtr<ID2D1Bitmap> d2d1Bitmap;
    D2D1_SIZE_F bitmapSize = { 0 };

    if (SUCCEEDED(hr))
    {
        hr = CreateD2D1BitmapFromFile(filename, &d2d1Bitmap);
        assert(SUCCEEDED(hr));
    }

    CComPtr<IDCompositionSurface> surfaceTile;

    if (SUCCEEDED(hr))
    {
        bitmapSize = d2d1Bitmap->GetSize();

        hr = _device->CreateSurface(
            static_cast<UINT>(bitmapSize.width),
            static_cast<UINT>(bitmapSize.height),
            DXGI_FORMAT_R8G8B8A8_UNORM,
            DXGI_ALPHA_MODE_IGNORE,
            &surfaceTile);
        assert(SUCCEEDED(hr));
    }

    CComPtr<IDXGISurface> dxgiSurface;
    POINT offset;

    if (SUCCEEDED(hr))
    {
        RECT rect = { 0, 0, static_cast<LONG>(bitmapSize.width), static_cast<LONG>(bitmapSize.height) };

        hr = surfaceTile->BeginDraw(&rect, __uuidof(IDXGISurface), reinterpret_cast<void**>(&dxgiSurface), &offset);
        assert(SUCCEEDED(hr));
    }

    CComPtr<ID2D1Bitmap1> d2d1Target;

    if (SUCCEEDED(hr))
    {
        FLOAT dpiX = 0.0f;
        FLOAT dpiY = 0.0f;

//        _d2d1Factory->GetDesktopDpi(&dpiX, &dpiY);
        float dpi = GetDpiForWindow(_hwnd);

        D2D1_BITMAP_PROPERTIES1 bitmapProperties = D2D1::BitmapProperties1(
            D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
            D2D1::PixelFormat(DXGI_FORMAT_R8G8B8A8_UNORM, D2D1_ALPHA_MODE_IGNORE),
            dpi,
            dpi);

        hr = _d2d1DeviceContext->CreateBitmapFromDxgiSurface(dxgiSurface, &bitmapProperties, &d2d1Target);
        assert(SUCCEEDED(hr));

        if (SUCCEEDED(hr))
        {
            _d2d1DeviceContext->SetTarget(d2d1Target);

            _d2d1DeviceContext->BeginDraw();

            _d2d1DeviceContext->DrawBitmap(
                d2d1Bitmap,
                D2D1::RectF(
                    offset.x + 0.0f,
                    offset.y + 0.0f,
                    offset.x + bitmapSize.width,
                    offset.y + bitmapSize.height));

            hr = _d2d1DeviceContext->EndDraw();
            assert(SUCCEEDED(hr));
        }

        surfaceTile->EndDraw();
    }

    if (SUCCEEDED(hr))
    {
        *bitmapWidth = static_cast<int>(bitmapSize.width);
        *bitmapHeight = static_cast<int>(bitmapSize.height);
        *surface = surfaceTile.Detach();
    }

    return hr;
}

HRESULT CAnimateWAM::CreateD2D1BitmapFromFile(const WCHAR* filename, ID2D1Bitmap** bitmap)
{
    HRESULT hr = (bitmap == nullptr) ? E_POINTER : S_OK;
    assert(SUCCEEDED(hr));

    if (SUCCEEDED(hr))
    {
        *bitmap = nullptr;

        hr = (_wicFactory == nullptr) ? E_UNEXPECTED : S_OK;
        assert(SUCCEEDED(hr));
    }

    CComPtr<IWICBitmapDecoder> wicBitmapDecoder;

    if (SUCCEEDED(hr))
    {
        hr = _wicFactory->CreateDecoderFromFilename(
            filename,
            nullptr,
            GENERIC_READ,
            WICDecodeMetadataCacheOnLoad,
            &wicBitmapDecoder);
        assert(SUCCEEDED(hr));
    }

    CComPtr<IWICBitmapFrameDecode> wicBitmapFrame;

    if (SUCCEEDED(hr))
    {
        hr = wicBitmapDecoder->GetFrame(0, &wicBitmapFrame);
        assert(SUCCEEDED(hr));
    }

    CComPtr<IWICFormatConverter> wicFormatConverter;

    if (SUCCEEDED(hr))
    {
        hr = _wicFactory->CreateFormatConverter(&wicFormatConverter);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = wicFormatConverter->Initialize(
            wicBitmapFrame,
            GUID_WICPixelFormat32bppPBGRA,
            WICBitmapDitherTypeNone,
            nullptr,
            0.0f,
            WICBitmapPaletteTypeMedianCut);
        assert(SUCCEEDED(hr));
    }

    CComPtr<IWICBitmap> wicBitmap;

    if (SUCCEEDED(hr))
    {
        hr = _wicFactory->CreateBitmapFromSource(wicFormatConverter, WICBitmapCacheOnLoad, &wicBitmap);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = _d2d1DeviceContext->CreateBitmapFromWicBitmap(wicBitmap, bitmap);
        assert(SUCCEEDED(hr));
    }

    return hr;
}



//-------------------------------------------------------
// Creates and initializes all the objects we need for the application
//--------------------------------------------------------

HRESULT CAnimateWAM::BeforeEnteringMessageLoop(const WCHAR* filename)
{
    HRESULT hr = 0;

//    hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
//    assert(SUCCEEDED(hr));

//    if (SUCCEEDED(hr))
//    {
//        hr = CreateApplicationWindow();
//    }

    if (SUCCEEDED(hr))
    {
        hr = CreateD3D11Device();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateD2D1Factory();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateD2D1Device();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateWICFactory();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateAnimationManager();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateAnimationTransitionLibrary();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateAnimationVariables();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateDCompositionDevice();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateDCompositionRenderTarget();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = CreateDCompositionVisualTreeFromFile(filename);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = AttachDCompositionVisualTreeToRenderTarget();
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        hr = _device->Commit();
        assert(SUCCEEDED(hr));
    }

    return hr;
}

//-----------------------------------------------------------
// Cleanup when we close the application
//-----------------------------------------------------------
void CAnimateWAM::AfterLeavingMessageLoop()
{
    DestroyDCompositionVisualTree();
    DestroyDCompositionRenderTarget();
    DestroyDCompositionDevice();
//    DestroyApplicationWindow();
    DestroyAnimationTransitionLibrary();
    DestroyAnimationVariables();
    DestroyAnimationManager();
//    DestroyApplicationWindow();
    DestroyWICFactory();
    DestroyD2D1Device();
    DestroyD2D1Factory();
    DestroyD3D11Device();
    CoUninitialize();
}


//---------------------------------------------------------------------
// Slides the tiles in the direction of the button press. WAM is 
// used to generate the animation curves while DirectComposition translates the visuals
// using those curves
//--------------------------------------------------------------------
HRESULT CAnimateWAM::Move(DIRECTION dir)
{
    HRESULT hr = ((_device == nullptr) || (_hwnd == NULL)) ? E_UNEXPECTED : S_OK;
    assert(SUCCEEDED(hr));

    CComPtr<IDCompositionAnimation> spAnimation;
    CComPtr<IDCompositionAnimation> slideAnimation;
    CComPtr<IDCompositionTranslateTransform> translateTransform;

    if (SUCCEEDED(hr))
    {
        // Create the animation curves using WAM
        // allow silient failure of this function to indicate end of transition.
        hr = CreateSlideAnimation(dir, &slideAnimation);
//        assert(SUCCEEDED(hr));    // allow silient failure of this function
    }

    if (SUCCEEDED(hr))
    {
        hr = _device->CreateTranslateTransform(&translateTransform);
        assert(SUCCEEDED(hr));
    }

    //Set DirectComposition translation animation using the curves propagated by WAM
    if (SUCCEEDED(hr))
    {
        hr = translateTransform->SetOffsetX(slideAnimation);
        assert(SUCCEEDED(hr));
    }

    if (SUCCEEDED(hr))
    {
        _visual->SetTransform(translateTransform);
        assert(SUCCEEDED(hr));
    }

    // Committing all changes to DirectComposition visuals in order for them to take effect visually
    if (SUCCEEDED(hr))
    {
        _device->Commit();
        assert(SUCCEEDED(hr));
    }

    return hr;
}
