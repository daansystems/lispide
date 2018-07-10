
class COpenDocumentList : public CRecentDocumentListBase<COpenDocumentList, MAX_PATH, 1, 32> {

public:
	static LPCTSTR GetRegKeyName()
	{
		return _T("Open Document List");
	}

	void Sort() {
		int i, j, flag = 1;    // set flag to 1 to start first pass
		CRecentDocumentListBase<COpenDocumentList, MAX_PATH, 1, 32>::_DocEntry temp; // holding variable
		int numLength = this->m_arrDocs.GetSize();
		for (i = 1; (i <= numLength) && flag; i++)
		{
			flag = 0;
			for (j = 0; j < (numLength - 1); j++)
			{
				if (lstrcmpi(this->m_arrDocs[j + 1].szDocName, this->m_arrDocs[j].szDocName) < 0)      // ascending order simply changes to <
				{
					temp = this->m_arrDocs[j];             // swap elements
					this->m_arrDocs[j] = this->m_arrDocs[j + 1];
					this->m_arrDocs[j + 1] = temp;
					flag = 1;               // indicates that a swap occurred.
				}
			}
		}
		return;   //arrays are passed to functions by address; nothing is returned
	}

};