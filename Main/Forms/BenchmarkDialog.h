/*
 Copyright (c) 2010 YourProduct Developers Association. All rights reserved.

 Governed by the YourProduct License 3.0 the full text of which is contained in
 the file License.txt included in YourProduct binary and source code distribution
 packages.
*/

#ifndef TC_HEADER_Main_Forms_BenchmarkDialog
#define TC_HEADER_Main_Forms_BenchmarkDialog

#include "Forms.h"
#include "Main/Main.h"

namespace YourProduct
{
	class BenchmarkDialog : public BenchmarkDialogBase
	{
	public:
		BenchmarkDialog (wxWindow *parent);

	protected:
		enum
		{
			ColumnAlgorithm = 0,
			ColumnEncryption,
			ColumnDecryption,
			ColumnMean
		};

		struct BenchmarkResult
		{
			wstring AlgorithmName;
			uint64 EncryptionSpeed;
			uint64 DecryptionSpeed;
			uint64 MeanSpeed;
		};

		void OnBenchmarkButtonClick (wxCommandEvent& event);
	};
}

#endif // TC_HEADER_Main_Forms_BenchmarkDialog
