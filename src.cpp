
#include "head.h"
#include "TDocStd_Application.hxx"
#include "TDataStd_Integer.hxx"
#include "BinDrivers.hxx"

#include <BRepPrimAPI_MakeCylinder.hxx>
#include <BRepPrimAPI_MakePrism.hxx>

#include <GC_MakeArcOfCircle.hxx>
#include <GC_MakeSegment.hxx>

#include <GCE2d_MakeSegment.hxx>

#include <gp.hxx>
#include <gp_Ax1.hxx>
#include <gp_Ax2.hxx>
#include <gp_Ax2d.hxx>
#include <gp_Dir.hxx>
#include <gp_Dir2d.hxx>
#include <gp_Pnt.hxx>
#include <gp_Pnt2d.hxx>
#include <gp_Trsf.hxx>
#include <gp_Vec.hxx>

#include <Geom_CylindricalSurface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Surface.hxx>
#include <Geom_TrimmedCurve.hxx>

#include <Geom2d_Ellipse.hxx>
#include <Geom2d_TrimmedCurve.hxx>

#include <TopExp_Explorer.hxx>

#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Face.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Shape.hxx>
#include <TopoDS_Compound.hxx>
#include "GeomProjLib.hxx"
#include "BRepBuilderAPI_MakeEdge.hxx"
#include "BRepBuilderAPI_MakeWire.hxx"
#include "BRepBuilderAPI_MakeFace.hxx"
#include "IntTools_EdgeEdge.hxx"


void testInteract()
{
	gp_Pnt pnt1(0, 0, 0);
	gp_Pnt pnt2(10, 0, 0);
	gp_Pnt pnt3(10, 10, 0);
	gp_Pnt pnt4(0, 10, 0);
	Handle(Geom_TrimmedCurve) curve1 =
		GC_MakeSegment(pnt1, pnt2);
	Handle(Geom_TrimmedCurve) curve2 =
		GC_MakeSegment(pnt2, pnt3);
	Handle(Geom_TrimmedCurve)curve3 =
		GC_MakeSegment(pnt3, pnt4);
	Handle(Geom_TrimmedCurve)curve4 =
		GC_MakeSegment(pnt4, pnt1);
	TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(curve1);
	TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(curve2);
	TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(curve3);
	TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(curve4);
	TopoDS_Wire wire = BRepBuilderAPI_MakeWire(edge1, edge2, edge3, edge4);
	IntTools_EdgeEdge edgeIntersect(edge1, edge2);
	edgeIntersect.Perform();
	if (edgeIntersect.IsDone())
	{
		IntTools_SequenceOfCommonPrts d = edgeIntersect.CommonParts();

	}
}



void testProject()
{
	gp_Pnt pnt1(0, 0, 0);
	gp_Pnt pnt2(10, 0, 0);
	gp_Pnt pnt3(10, 10, 0);
	gp_Pnt pnt4(0, 10, 0);
	Handle(Geom_TrimmedCurve) curve1 =
		GC_MakeSegment(pnt1, pnt2);
	Handle(Geom_TrimmedCurve) curve2 =
		GC_MakeSegment(pnt2, pnt3);
	Handle(Geom_TrimmedCurve)curve3 =
		GC_MakeSegment(pnt3, pnt4);
	Handle(Geom_TrimmedCurve)curve4 =
		GC_MakeSegment(pnt4, pnt1);
	TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(curve1);
	TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(curve2);
	TopoDS_Edge edge3 = BRepBuilderAPI_MakeEdge(curve3);
	TopoDS_Edge edge4 = BRepBuilderAPI_MakeEdge(curve4);
	TopoDS_Wire wire = BRepBuilderAPI_MakeWire(edge1, edge2, edge3, edge4);

	TopoDS_Face face = BRepBuilderAPI_MakeFace(wire);
	//return face;
	gp_Vec aPrimsVec = { 0,0,8 };
	TopoDS_Shape shape = BRepPrimAPI_MakePrism(face, aPrimsVec);
	for (TopExp_Explorer anExp(shape, TopAbs_VERTEX); anExp.More(); anExp.Next())
	{
		const TopoDS_Shape& aShape = anExp.Current();
		aShape.Convex();
	}
	for (TopExp_Explorer anExp(shape, TopAbs_FACE); anExp.More(); anExp.Next())
	{
		const TopoDS_Shape& aShape = anExp.Current();
		TopoDS_Face face = TopoDS::Face(aShape);
		// 获取面的几何信息
		TopLoc_Location location;
		Handle(Geom_Surface) surface = BRep_Tool::Surface(face, location);
		GeomProjLib::Project(curve3, surface);
	}

}

int testFileSave()
{
	Handle(TDocStd_Application) app
		= new TDocStd_Application;

	BinDrivers::DefineFormat(app);

	Handle(TDocStd_Document) doc;
	app->NewDocument("BinOcaf",doc);

	if (doc.IsNull())
	{
		std::cout << "Error:cannot create an OCAF document." << std::endl;
		return 1;
	}
	/*
	* 0
	* |
	* 0->1 (0:1) mainlabel
	*/
	TDF_Label mainLab = doc->Main();
	TDataStd_Integer::Set(mainLab, 199);
	PCDM_StoreStatus sstatus = app->SaveAs(doc,"test.cbf");
	if (sstatus != PCDM_SS_OK)
	{
		app->Close(doc);
		std::cout << "can not write OCAF document" << std::endl;
		return 1;
	}

	app->Close(doc);


	return 0;

}
