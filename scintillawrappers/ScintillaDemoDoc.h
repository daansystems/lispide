#pragma once

#include "ScintillaDocView.h"

class CScintillaDemoDoc : public CScintillaDoc
{
protected: //create from serialization only
  DECLARE_DYNCREATE(CScintillaDemoDoc)

protected:
  DECLARE_MESSAGE_MAP()
};
