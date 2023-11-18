#include "Viewer.h"

// OCC
#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepTools.hxx>

//
#include <BRepAlgoAPI_Fuse.hxx>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_Transform.hxx>

#include <BRepFilletAPI_MakeFillet.hxx>

#include <BRepLib.hxx>

#include <BRepOffsetAPI_MakeThickSolid.hxx>
//#include <BRepOffsetAPI_ThruSections.hxx>

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
#include "TopLoc_Location.hxx"

extern int testFileSave();
TopoDS_Shape testTopo();
TopoDS_Shape FilletedBox(const Standard_Real a,
	const Standard_Real  b,
	const Standard_Real  c,
	const Standard_Real  r);

TopoDS_Shape OnEvolvedblend1();

int main(int argc, char** argv)
{
	Viewer vout(50, 50, 500, 500);
	TopoDS_Shape sh;

	if (argc > 1) {
		BRep_Builder bbuilder;


		if (BRepTools::Read(sh, argv[1], bbuilder)) {
			return 1;
		}
	}

	/*
	// Make shape.
	BRepPrimAPI_MakeBox mkBox(10, 20, 30);
	mkBox.Build();
	//
	if (!mkBox.IsDone())
		return 1;
	//
	const TopoDS_Shape& shape = mkBox.Shape();

	// Visualize.
	vout << shape;
	*/
	//vout << BRepPrimAPI_MakeBox(10, 20, 30)
	//	<< sh;
	//vout << testTopo();
	//vout << FilletedBox(3, 4, 5, 1);
	testFileSave();
	vout << OnEvolvedblend1();

	vout.StartMessageLoop();
	return 0;
}

TopoDS_Shape OnEvolvedblend1()
{
	TopoDS_Shape theBox = BRepPrimAPI_MakeBox(200, 200, 200);

	BRepFilletAPI_MakeFillet  Rake(theBox);
	ChFi3d_FilletShape  FSh = ChFi3d_Rational;
	Rake.SetFilletShape(FSh);

	TColgp_Array1OfPnt2d  ParAndRad(1, 6);
	ParAndRad(1).SetCoord(0., 10.);
	ParAndRad(1).SetCoord(50., 20.);
	ParAndRad(1).SetCoord(70., 20.);
	ParAndRad(1).SetCoord(130., 60.);
	ParAndRad(1).SetCoord(160., 30.);
	ParAndRad(1).SetCoord(200., 20.);

	TopExp_Explorer  ex(theBox, TopAbs_EDGE);
	Rake.Add(ParAndRad, TopoDS::Edge(ex.Current()));
	TopoDS_Shape  evolvedBox = Rake.Shape();
	return evolvedBox;
}

TopoDS_Shape FilletedBox(const Standard_Real a,
	const Standard_Real  b,
	const Standard_Real  c,
	const Standard_Real  r)
{
	TopoDS_Solid Box = BRepPrimAPI_MakeBox(a, b, c);
	BRepFilletAPI_MakeFillet  MF(Box);

	// add all the edges  to fillet 
	TopExp_Explorer  ex(Box, TopAbs_EDGE);
	while (ex.More())
	{
		MF.Add(r, TopoDS::Edge(ex.Current()));
		ex.Next();
	}
	return MF.Shape();
}
TopoDS_Shape testTopo()
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

	}
	return shape;

}

