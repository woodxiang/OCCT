//=============================================================================
//
// SHICHUANG CONFIDENTIAL
// __________________
//
//  [2016] - [2021] SHICHUANG Co., Ltd.
//  All Rights Reserved.
//
//=============================================================================

#include "stdafx.h"
#include "View.h"
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_Viewer.hxx>
#include <WNT_Window.hxx>
#include <V3d_View.hxx>
#include <AIS_InteractiveContext.hxx>
#include <BRepPrimAPI_MakeWedge.hxx>
#include <AIS_Shape.hxx>
#include <RWStl.hxx>
#include <XSDRAWSTLVRML_DataSource.hxx>
#include <MeshVS_Mesh.hxx>
#include <MeshVS_Drawer.hxx>
#include <MeshVS_DrawerAttribute.hxx>
#include <MeshVS_MeshPrsBuilder.hxx>

LRESULT CView::OnCreate(UINT, WPARAM, LPARAM, BOOL&)
{
	Handle(OpenGl_GraphicDriver) graphicDriver = new OpenGl_GraphicDriver(Handle(Aspect_DisplayConnection)());
	Handle(V3d_Viewer) viewer = new V3d_Viewer(graphicDriver);

	viewer->SetDefaultLights();
	viewer->SetLightOn();
	viewer->SetDefaultBackgroundColor(Quantity_NOC_BLACK);

	Handle(WNT_Window) window = new WNT_Window(m_hWnd);
	Handle(V3d_View) view = viewer->CreateView();
	view->SetBackgroundColor(Quantity_NOC_BLACK);
	view->SetImmediateUpdate(false);
	view->SetWindow(window);
	view->Camera()->SetProjectionType(Graphic3d_Camera::Projection_Orthographic);

	Handle(AIS_InteractiveContext) context = new AIS_InteractiveContext(viewer);

	//BRepPrimAPI_MakeWedge wedgeMaker(10, 20, 30, 3);
	//TopoDS_Solid shape = wedgeMaker.Solid();

	//Handle(AIS_Shape) shapePres = new AIS_Shape(shape);

	// context->Display(shapePres, AIS_Shaded, 0, true);

	// Load STL

	std::string stlFile = R"(D:\HPDC_STL\Clutch\cast.stl)";

	auto triangulation = RWStl::ReadFile(stlFile.c_str());

	Handle_XSDRAWSTLVRML_DataSource dataSource =
		new XSDRAWSTLVRML_DataSource(triangulation);

	Handle(MeshVS_Mesh) mesh = new MeshVS_Mesh();
	mesh->SetDataSource(dataSource);
	mesh->AddBuilder(new MeshVS_MeshPrsBuilder(mesh), Standard_True);

	auto drawer = mesh->GetDrawer();
	drawer->SetBoolean(MeshVS_DA_DisplayNodes, Standard_False);
	drawer->SetBoolean(MeshVS_DA_ShowEdges, Standard_False);
	mesh->SetMeshSelMethod(MeshVS_MSM_BOX);
	mesh->SetDisplayMode(MeshVS_DMF_Shading);

	context->Display(mesh, Standard_True);

	view->Redraw();
	view->Invalidate();

	m_view = view;
	m_context = context;

	return 0;
}

LRESULT CView::OnPaint(UINT, WPARAM, LPARAM, BOOL&)
{
	CPaintDC dc(m_hWnd);

	if (!m_view.IsNull()) {
		m_view->InvalidateImmediate();
		FlushViewEvents(m_context, m_view, true);
		m_view->Redraw();
	}
	return 0;
}

LRESULT CView::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	int width = LOWORD(lParam);
	int height = HIWORD(lParam);

	if (width != 0 && height != 0 && !m_view.IsNull())
	{
		m_view->Window()->DoResize();
		m_view->MustBeResized();
		m_view->FitAll();
		m_view->Invalidate();
	}

	return 0;
}

LRESULT CView::OnLeftButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	PressMouseButton(Graphic3d_Vec2i((int)(LOWORD(lParam)), (int)(HIWORD(lParam))), Aspect_VKeyMouse_LeftButton, wParam, false);
	UpdateView();
	return LRESULT();
}

LRESULT CView::OnLeftButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	auto flags = WNT_Window::MouseKeyFlagsFromEvent(wParam);
	ReleaseMouseButton(Graphic3d_Vec2i((int)(LOWORD(lParam)), (int)(HIWORD(lParam))), Aspect_VKeyMouse_LeftButton, flags, false);
	UpdateView();

	return LRESULT();
}

LRESULT CView::OnRightButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return LRESULT();
}

LRESULT CView::OnRightButtonUp(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return LRESULT();
}

LRESULT CView::OnMouseMove(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	TRACKMOUSEEVENT mouseEvent;
	mouseEvent.cbSize = sizeof(mouseEvent);
	mouseEvent.dwFlags = TME_LEAVE;
	mouseEvent.hwndTrack = m_hWnd;
	mouseEvent.dwHoverTime = HOVER_DEFAULT;
	if (!::_TrackMouseEvent(&mouseEvent)) {
		// TODO: Trace errro;
	}

	auto flags = WNT_Window::MouseKeyFlagsFromEvent(wParam);
	if (UpdateMousePosition(Graphic3d_Vec2i((int)(LOWORD(lParam)), (int)(HIWORD(lParam))), PressedMouseButtons(), flags, false)) {
		UpdateView();
	}
	return LRESULT();
}

LRESULT CView::OnMouseWheel(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	return LRESULT();
}

inline void CView::UpdateView()
{
	if (!m_view.IsNull()) {
		Invalidate(FALSE);
		UpdateWindow();
	}
}