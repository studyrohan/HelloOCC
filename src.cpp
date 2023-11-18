

#include "TDocStd_Application.hxx"
#include "TDataStd_Integer.hxx"
#include "BinDrivers.hxx"

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
