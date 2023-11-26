

#include "head.h"

#include <Geom_BSplineSurface.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array2OfReal.hxx>
#include <GeomAPI_PointsToBSpline.hxx>
#include "BRepBuilderAPI_MakeFace.hxx"
#include "TopoDS_Face.hxx"
#include "BRepPrimAPI_MakePrism.hxx"
#include <ShapeAnalysis_Edge.hxx>
#include <vector>
#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TopoDS_Edge.hxx"

//TopoDS_Shape CreateNurbs()
//{
//	// 定义控制点
//	std::vector<gp_Pnt> controlPoints;
//	controlPoints.push_back(gp_Pnt(0, 0, 0));
//	controlPoints.push_back(gp_Pnt(1, 0, 0));
//	controlPoints.push_back(gp_Pnt(2, 0, 0));
//	controlPoints.push_back(gp_Pnt(3, 0, 0));
//	controlPoints.push_back(gp_Pnt(0, 1, 1));
//	controlPoints.push_back(gp_Pnt(1, 1, 2));
//	controlPoints.push_back(gp_Pnt(2, 1, 1));
//	controlPoints.push_back(gp_Pnt(3, 1, 0));
//	controlPoints.push_back(gp_Pnt(0, 2, 0));
//	controlPoints.push_back(gp_Pnt(1, 2, 0));
//	controlPoints.push_back(gp_Pnt(2, 2, 0));
//	controlPoints.push_back(gp_Pnt(3, 2, 0));
//	controlPoints.push_back(gp_Pnt(0, 3, 0));
//	controlPoints.push_back(gp_Pnt(1, 3, 0));
//	controlPoints.push_back(gp_Pnt(2, 3, 0));
//	controlPoints.push_back(gp_Pnt(3, 3, 0));
//
//	// 定义节点向量
//	std::vector<double> knotsU = { 0, 0, 0, 0, 1, 1, 1, 1 };
//	std::vector<double> knotsV = { 0, 0, 0, 0, 1, 1, 1, 1 };
//
//	// 定义阶数
//	int degreeU = 3;
//	int degreeV = 3;
//
//	// 定义UMults和VMults
//	std::vector<int> uMults = { 4, 4 };
//	std::vector<int> vMults = { 4, 4 };
//
//	// 构造B样条曲面
//	Handle(Geom_BSplineSurface) bsplineSurface = new Geom_BSplineSurface(controlPoints, knotsU, knotsV, uMults, vMults, degreeU, degreeV);
//
//	// 输出曲面信息
//	std::cout << "B-Spline Surface:" << std::endl;
//	std::cout << "Degree U: " << bsplineSurface->UDegree() << std::endl;
//	std::cout << "Degree V: " << bsplineSurface->VDegree() << std::endl;
//	std::cout << "Number of Control Points U: " << bsplineSurface->NbUPoles() << std::endl;
//	std::cout << "Number of Control Points V: " << bsplineSurface->NbVPoles() << std::endl;
//	std::cout << "Number of Knots U: " << bsplineSurface->NbUKnots() << std::endl;
//	std::cout << "Number of Knots V: " << bsplineSurface->NbVKnots() << std::endl;
//	BRepBuilderAPI_MakeFace faceMaker(bsplineSurface, Precision::Confusion());
//
//	// 获取面的几何体
//	TopoDS_Face face = faceMaker.Face();
//	// 将面转换为其他几何体，比如棱柱体
//	gp_Vec aVector(0, 0, 1); // 棱柱体的方向
//	BRepPrimAPI_MakePrism prismMaker(face, aVector);
//	TopoDS_Shape prismShape = prismMaker.Shape();
//	return prismShape;
//
//}


TopoDS_Shape testNurbs()
{
	// 创建一个面
	TColgp_Array2OfPnt poles(1, 2, 1, 2);
	poles(1, 1) = gp_Pnt(0.0, 0.0, 0.0);
	poles(2, 1) = gp_Pnt(1.0, 0.0, 0.0);
	poles(1, 2) = gp_Pnt(0.0, 1.0, 0.0);
	poles(2, 2) = gp_Pnt(1.0, 1.0, 0.0);

	TColStd_Array1OfReal knots(1, 2);
	knots(1) = 0.0;
	knots(2) = 1.0;

	TColStd_Array1OfInteger mults(1, 2);
	mults(1) = 2;
	mults(2) = 2;

	Handle(Geom_BSplineSurface) nurbsSurface = new Geom_BSplineSurface(poles, knots, knots, mults, mults, 1, 1);
	BRepBuilderAPI_MakeFace faceMaker(nurbsSurface, Precision::Confusion());
	TopoDS_Face face = faceMaker.Face();
	gp_Vec aVector(0, 0, 1); // 棱柱体的方向
	BRepPrimAPI_MakePrism prismMaker(face, aVector);
	TopoDS_Shape prismShape = prismMaker.Shape();

	for (TopExp_Explorer anExp(prismShape, TopAbs_EDGE); anExp.More(); anExp.Next())
	{
		const TopoDS_Shape& aShape = anExp.Current();
		TopoDS_Edge edge = TopoDS::Edge(aShape);
		Handle(Geom_Curve) C3d;
		Standard_Real cf, cl;
		ShapeAnalysis_Edge AnalyseEdge;
		if (AnalyseEdge.HasCurve3d(edge))
		{
			Standard_Boolean b= AnalyseEdge.Curve3d(edge, C3d, cf, cl);
			if (b)
			{
				break;
			}
		}

	}


	return prismShape;

	//// 定义控制点
	//TColgp_Array2OfPnt Poles(1, 4, 1, 4);
	//Poles.SetValue(1, 1, gp_Pnt(0, 0, 0));
	//Poles.SetValue(1, 2, gp_Pnt(1, 0, 0));
	//Poles.SetValue(1, 3, gp_Pnt(2, 0, 0));
	//Poles.SetValue(1, 4, gp_Pnt(3, 0, 0));
	//Poles.SetValue(2, 1, gp_Pnt(0, 1, 1));
	//Poles.SetValue(2, 2, gp_Pnt(1, 1, 1));
	//Poles.SetValue(2, 3, gp_Pnt(2, 1, 1));
	//Poles.SetValue(2, 4, gp_Pnt(3, 1, 1));
	//Poles.SetValue(3, 1, gp_Pnt(0, 2, 2));
	//Poles.SetValue(3, 2, gp_Pnt(1, 2, 2));
	//Poles.SetValue(3, 3, gp_Pnt(2, 2, 2));
	//Poles.SetValue(3, 4, gp_Pnt(3, 2, 2));
	//Poles.SetValue(4, 1, gp_Pnt(0, 3, 3));
	//Poles.SetValue(4, 2, gp_Pnt(1, 3, 3));
	//Poles.SetValue(4, 3, gp_Pnt(2, 3, 3));
	//Poles.SetValue(4, 4, gp_Pnt(3, 3, 3));

	//// 定义节点
	//TColStd_Array1OfReal UKnots(1, 8);
	//UKnots.SetValue(1, 0);
	//UKnots.SetValue(2, 0);
	//UKnots.SetValue(3, 1);
	//UKnots.SetValue(4, 1);
	//UKnots.SetValue(5, 2);
	//UKnots.SetValue(6, 2);
	//UKnots.SetValue(7, 3);
	//UKnots.SetValue(8, 3);

	//TColStd_Array1OfReal VKnots(1, 8);
	//VKnots.SetValue(1, 0);
	//VKnots.SetValue(2, 0);
	//VKnots.SetValue(3, 1);
	//VKnots.SetValue(4, 1);
	//VKnots.SetValue(5, 2);
	//VKnots.SetValue(6, 2);
	//VKnots.SetValue(7, 3);
	//VKnots.SetValue(8, 3);

	//// 定义度数
	//Standard_Integer UDegree = 3;
	//Standard_Integer VDegree = 3;

	//// 定义UMults和VMults
	//TColStd_Array1OfInteger uMults = { 4, 4 };
	//TColStd_Array1OfInteger vMults = { 4, 4 };

	//Handle(Geom_BSplineSurface) nurbsSurface = new Geom_BSplineSurface(Poles, UKnots, VKnots,uMults,vMults, UDegree, VDegree);

	//// 创建NURBS曲面
	//BRepBuilderAPI_MakeFace faceMaker(nurbsSurface, Precision::Confusion());

	//// 获取面的几何体
	//TopoDS_Face face = faceMaker.Face();
	//// 将面转换为其他几何体，比如棱柱体
	//gp_Vec aVector(0, 0, 1); // 棱柱体的方向
	//BRepPrimAPI_MakePrism prismMaker(face, aVector);
	//TopoDS_Shape prismShape = prismMaker.Shape();
	//return prismShape;
}
