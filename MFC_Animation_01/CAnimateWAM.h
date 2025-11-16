#pragma once

#include <atlbase.h>
#include <dcomp.h>
#include <d3d11_1.h>
#include <d2d1_1.h>
#include <uianimation.h>

enum DIRECTION
{
    stopForward = -1,
    stopBackward = 1,
    forward,
    backward,
};


class CAnimateWAM
{
private:
    HRESULT CreateD3D11Device();
    HRESULT CreateD2D1Factory();
    HRESULT CreateD2D1Device();
    HRESULT CreateWICFactory();
    HRESULT CreateAnimationManager();
    HRESULT CreateAnimationTransitionLibrary();
    HRESULT CreateAnimationVariables();
    HRESULT CreateDCompositionDevice();
    HRESULT CreateDCompositionRenderTarget();
    HRESULT CreateDCompositionVisualTree();
    HRESULT AttachDCompositionVisualTreeToRenderTarget();
    HRESULT DetachDCompositionVisualTreeToRenderTarget();
    HRESULT CreateSlideAnimation(DIRECTION dir, IDCompositionAnimation** slideAnimation);
    void DestroyDCompositionVisualTree();
    void DestroyDCompositionRenderTarget();
    void DestroyDCompositionDevice();
    void DestroyAnimationVariables();
    void DestroyAnimationTransitionLibrary();
    void DestroyAnimationManager();
    void DestroyWICFactory();
    void DestroyD2D1Device();
    void DestroyD2D1Factory();
    void DestroyD3D11Device();

    CComPtr<ID3D11Device> _d3d11Device;
    CComPtr<ID3D11DeviceContext> _d3d11DeviceContext;

    CComPtr<ID2D1Factory1> _d2d1Factory;

    CComPtr<ID2D1Device> _d2d1Device;
    CComPtr<ID2D1DeviceContext> _d2d1DeviceContext;

    CComPtr<IWICImagingFactory> _wicFactory;

    CComPtr<IUIAnimationManager2> _manager;
    CComPtr<IUIAnimationTransitionLibrary2> _transitionLibrary;
    CComPtr<IUIAnimationVariable2> _animationVariable;

    CComPtr<IDCompositionDevice> _device;
    CComPtr<IDCompositionTarget> _target;
    CComPtr<IDCompositionVisual> _visual;
    CComPtr<IDCompositionVisual> _visualChild[30];

    HWND _hwnd;
    HBRUSH _hbrush;

    int _bitmapWidth;
    int _bitmapHeight;

    int _sizeWidow;
    float _spacingTile;

public:
	CAnimateWAM(int sizeWindow = 0, float spacingTile = 0.0);
	~CAnimateWAM();

    void SetHwndSizeTileSpacing(HWND hWnd, int sizeWindow = 0, float spacingTile = 0.0f);

    HRESULT BeforeEnteringMessageLoop();
    void AfterLeavingMessageLoop();
    HRESULT Move(DIRECTION dir);

    HRESULT CreateSurfaceFromFile(const WCHAR* filename, int* bitmapWidth, int* bitmapHeight, IDCompositionSurface** surface);
    HRESULT CreateD2D1BitmapFromFile(const WCHAR* filename, ID2D1Bitmap** bitmap);


};

