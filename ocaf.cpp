/*
*    Copyright (c) 2013 eddy All Rights Reserved.
*
*        File    : ocaf.cpp
*        Author  : eddyyu136@gmail.com
*        Date    : 2023-11-19 
*        Version : 1.0v
*
*    Description : Demonstrate ocaf ability of occ
*/

#include "head.h"
#include "TDocStd_Application.hxx"
#include "TDataStd_Integer.hxx"
#include "BinDrivers.hxx"
#include "TopoDS_Shape.hxx"
#include "TNaming_NamedShape.hxx"
#include "TNaming_Builder.hxx"
#include "TDataStd_Name.hxx"
#include "BRepPrimAPI_MakeBox.hxx"

#include "TopExp_Explorer.hxx"
#include "TopoDS.hxx"
#include "TDataXtd_Triangulation.hxx"
#include "TDF_Reference.hxx"
#include "BRepMesh_IncrementalMesh.hxx"
#include "TDataStd_IntPackedMap.hxx"
#include "TColStd_PackedMapOfInteger.hxx"
#include "TDF_TagSource.hxx"
#include "TDF_ChildIterator.hxx"

/*draw exe command
* to deal cbd extension file
* Open
* DFBrowse
* Close
*/

class IPart;

class IFeature
{
public:
	IFeature(const TDF_Label& label):m_root(label)
	{}
public:
	void SetFaces(const TColStd_PackedMapOfInteger& fids)
	{
		Handle(TDataStd_IntPackedMap) attr = TDataStd_IntPackedMap::Set(m_root);
		attr->ChangeMap(fids);
	}

private:
	TDF_Label m_root;
};


class IPart
{
public:
	IPart(const TDF_Label& label) :m_root(label)
	{
		TDataStd_Name::Set(label, "Part");
	}
	const TDF_Label& GetLabel()const
	{
		return m_root;
	}
	void SetShape(const TopoDS_Shape& shape)
	{
		TNaming_Builder builder(m_root);
		builder.Generated(shape);

		BRepMesh_IncrementalMesh meshGen(shape, 1.0);
	}

	TopoDS_Shape GetShape()const
	{
		Handle(TNaming_NamedShape) attr;
		if (!m_root.FindAttribute(TNaming_NamedShape::GetID(), attr))
		{
			return TopoDS_Shape();
		}
		return attr->Get();
	}

	void SetColor(const unsigned r, const unsigned g, const unsigned b)
	{
		int iColor = (r << 16) + (g << 8) + b;
		TDataStd_Integer::Set(m_root, iColor);
	}
	bool GetColor(unsigned& r, unsigned& g, unsigned& b)
	{
		Handle(TDataStd_Integer)attr;
		if (!m_root.FindAttribute(TDataStd_Integer::GetID(), attr))
		{
			return false;
		}
		int iColor = attr->Get();
		r = (iColor >> 16) && 0xff;
		g = (iColor >> 8) && 0xff;
		b = (iColor) && 0xff;
		return true;
	}

	IFeature CreateFeature(const TColStd_PackedMapOfInteger& fids)
	{
		TDF_Label featureRootLab = TDF_TagSource::NewChild(m_root);
		IFeature feature(featureRootLab);
		feature.SetFaces(fids);
		return feature;
	}

	void GetFeatures(std::vector<IFeature>& features)
	{
		for (TDF_ChildIterator cit(m_root); cit.More(); cit.Next())
		{
			features.push_back(IFeature(cit.Value()));
		}
	}
private:
	TDF_Label m_root;
};


class IMesh
{
public:
	IMesh(const TDF_Label& label) :m_root(label)
	{

	}
public:
	void Create(const IPart& part)
	{
		TopoDS_Shape partShape = part.GetShape();
		Handle(Poly_Triangulation) mesh;
		for (TopExp_Explorer exp(partShape,TopAbs_FACE);exp.More();exp.Next())
		{
			const TopoDS_Face& face = TopoDS::Face(exp.Current());
			TopLoc_Location loc;
			mesh = BRep_Tool::Triangulation(face, loc);
			break;
		}
		if (mesh.IsNull())
		{
			return;
		}
		TDataXtd_Triangulation::Set(m_root, mesh);

		TDF_Reference::Set(m_root, part.GetLabel());
	}
private:
	TDF_Label m_root;
};


int testOCAF()
{
	Handle(TDocStd_Application) app
		= new TDocStd_Application;

	BinDrivers::DefineFormat(app);

	Handle(TDocStd_Document) doc;
	app->NewDocument("BinOcaf", doc);

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
	TDF_Label partsLab = doc->Main();
	TDataStd_Integer::Set(partsLab, 199);
	TDF_Label meshLab = partsLab.Father().FindChild(2);
	IPart part1(TDF_TagSource::NewChild(partsLab));
	IPart part2(TDF_TagSource::NewChild(partsLab));
	IPart part3(TDF_TagSource::NewChild(partsLab));

	part2.SetColor(255, 0, 0);
	part2.SetShape(BRepPrimAPI_MakeBox(200, 200, 200));

	TColStd_PackedMapOfInteger fids1,fids2;
	fids1.Add(1);
	fids1.Add(2);
	fids1.Add(3);
	fids2.Add(1);
	fids2.Add(2);
	fids2.Add(3);
	part3.CreateFeature(fids1);
	part3.CreateFeature(fids2);

	std::vector<IFeature>part3Features;
	part3.GetFeatures(part3Features);

	IMesh mesh1(partsLab);
	mesh1.Create(part1);



	PCDM_StoreStatus sstatus = app->SaveAs(doc, "test.cbf");
	if (sstatus != PCDM_SS_OK)
	{
		app->Close(doc);
		std::cout << "can not write OCAF document" << std::endl;
		return 1;
	}

	app->Close(doc);


	return 0;

}