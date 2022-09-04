/*
Module : ScintillaCtrl.cpp
Purpose: Defines the implementation for an MFC wrapper class for the Scintilla edit control (www.scintilla.org)
Created: PJN / 19-03-2004
History: PJN / 19-03-2004 1. Initial implementation synchronized to the v1.59 release of Scintilla
         PJN / 19-03-2004 1. Updated the sample app's Find Next and Find Previous marker functions. Now correctly
                          goes to the next and previous markers when a marker is on the current line.
                          2. Updated the sample app's passing of messages to Scintilla from the top level
                          CMainFrame window
         PJN / 06-06-2004 1. Updated class to work with Scintilla v1.61
         PJN / 20-12-2004 1. Updated class to work with Scintilla v1.62.
                          2. Sample app now includes a common control 6 manifest
                          3. Sample app now includes an example of scintilla autocompletion. When you type 
                          "scintilla is " case insensitively a autocompletion list is displayed which allows 
                          "very cool", "easy" or "way cool!!" to be entered. 
                          4. Sample app now includes an example of scintilla calltips. Whenever you hover
                          over text which is "author " case insensitively, a call tip with the text 
                          "PJ Naughter" is displayed.
         PJN / 10-07-2005 1. Updated class to work with Scintilla v1.64.
                          2. Fixed a number of warnings when the code is compiled using Visual Studio .NET 2003. 
         PJN / 03-01-2006.1. Updated class to work with Scintilla v1.67. New messages wrapped include: 
                          SCI_MARKERADDSET, SCI_SETPASTECONVERTENDINGS, SCI_GETPASTECONVERTENDINGS,
                          SCI_SELECTIONDUPLICATE and SCI_GETSTYLEBITSNEEDED. 
                          2. Updated copyright messages
         PJN / 14-03-2006 1. Updated class to work with Scintilla v1.68. New messages wrapped include:
                          SCI_CALLTIPUSESTYLE, SCI_SETCARETLINEBACKALPHA and SCI_GETCARETLINEBACKALPHA.
         PJN / 05-06-2006 1. Updated class to work with Scintilla v1.69. New messages wrapped include:
                          SCI_MARKERSETALPHA, SCI_GETSELALPHA and SCI_SETSELALPHA. 
         PJN / 06-06-2006 1. Updated the wrapper class to work correctly when compiled for Unicode.
         PJN / 29-06-2006 1. Code now uses new C++ style casts rather than old style C casts where necessary. 
                          2. Optimized CScintillaCtrl constructor code
                          3. Updated the code to clean compile in VC 2005
                          4. Fixed a bug in the sample program when you invoke Print Preview when compiled
                          using VC 2005
         PJN / 27-07-2006 1. Minor update to the sample app to fix an ASSERT related to the formatting of the
                          IDR_SCINTITYPE string resource. Thanks to Matt Spear for reporting this issue.
         PJN / 17-09-2006 1. Fixed a bug in UTF82W (and W2UTF8) where if GetLine is called in a Unicode build
                          for the end of the file (i.e. a line having a length of 0), the UTF82W function would
                          allocate no buffer, but still erroneously write a one character terminating null. In 
                          addition, the caller (GetLine) will try to deallocate the buffer that was never 
                          allocated. Thanks to Scott Kelley for spotting this nasty bug.
                          2. Added of a GetLineEx method which explicitly sets the first WORD value in the text 
                          string to the maximum size. This avoids client code from having to deal with the
                          weird semantics of the EM_GETLINE message. Thanks to Scott Kelley for providing this
                          nice addition.
                          3. Verified code implements all the functionality of Scintilla v1.71
         PJN / 11-06-2007 1. Updated copyright details.
                          2. CScintillaCtrl::GetSelText now uses CString::GetBufferSetLength to avoid having to
                          allocate an intermediate buffer. Thanks to Jochen Neubeck for reporting this 
                          optimization
                          3. Addition of a SCINTILLACTRL_EXT_CLASS preprocessor macro to allow the classes to be 
                          more easily used in an extension DLL.
                          4. Updated class to work with Scintilla v1.73. New messages wrapped include:
                          SCI_STYLEGETFORE, SCI_STYLEGETBACK, SCI_STYLEGETBOLD, SCI_STYLEGETITALIC, SCI_STYLEGETSIZE,
                          SCI_STYLEGETFONT, SCI_STYLEGETEOLFILLED, SCI_STYLEGETUNDERLINE, SCI_STYLEGETCASE,
                          SCI_STYLEGETCHARACTERSET, SCI_STYLEGETVISIBLE, SCI_STYLEGETCHANGEABLE, SCI_STYLEGETHOTSPOT,
                          SCI_GETSELEOLFILLED, SCI_SETSELEOLFILLED, SCI_GETHOTSPOTACTIVEFORE, SCI_GETHOTSPOTACTIVEBACK,
                          SCI_GETHOTSPOTACTIVEUNDERLINE & SCI_GETHOTSPOTSINGLELINE
         PJN / 28-11-2007 1. Updated class to work with Scintilla v1.75. New messages wrapped include: SCI_INDICSETUNDER, 
                          SCI_INDICGETUNDER, new behavior for SCI_SETINDENTATIONGUIDES & SCI_GETINDENTATIONGUIDES, 
                          SCI_SETSCROLLWIDTHTRACKING, SCI_GETSCROLLWIDTHTRACKING, SCI_DELWORDRIGHTEND, SCI_SETCARETSTYLE, 
                          SCI_GETCARETSTYLE, SCI_SETINDICATORCURRENT, SCI_SETINDICATORVALUE, SCI_INDICATORFILLRANGE, 
                          SCI_INDICATORCLEARRANGE, SCI_INDICATORALLONFOR, SCI_INDICATORVALUEAT, SCI_INDICATORSTART, 
                          SCI_INDICATOREND, SCI_SETPOSITIONCACHE & SCI_GETPOSITIONCACHE.
                          2. The auto completion sample in CScintillaDemoView::OnCharAdded has been extended to show
                          another style of auto completion. Thanks to Alessandro Limonta for suggesting this update.
         PJN / 19-03-2008 1. Updated class to work with Scintilla v1.76. New messages wrapped include: SCI_COPYALLOWLINE.
                          2. Updated copyright details.
                          3. Updated code to clean compile on VC 2008.
                          4. Removed VC 6 style classwizard comments from the code.
                          5. Updated the sample apps document icon.
                          6. Fixed a level 4 warning when the code is compiled on VC 6.
         PJN / 15-06-2008 1. Code now compiles cleanly using Code Analysis (/analyze)
                          2. Updated code to compile correctly using _ATL_CSTRING_EXPLICIT_CONSTRUCTORS define
                          3. The code now only supports VC 2005 or later.
         PJN / 01-11-2008 1. Updated class to work with Scintilla v1.77. New messages wrapped include: 
                          SCI_GETCHARACTERPOINTER, SCI_SETKEYSUNICODE & SCI_GETKEYSUNICODE
                          2. Reworked all the key Unicode functions which expose string length management and 
                          reimplemented them to use CStringW output parameters. Equivalent ASCII versions have also been 
                          provided. This new approach helps to raise the level of abstraction provided by the wrapper 
                          class. In the process the need for the GetLineEx function has been removed. Thanks to Alexei 
                          Letov for prompting this update.
         PJN / 20-01-2009 1. Updated copyright details.
         PJN / 03-10-2009 1. Fixed a bug in CScintillaCtrl::Create where a crash can occur in a Unicode build if the CreateEx
                          call fails (for example, if the Scintilla DLL was not loaded). Thanks to Simon Smith for reporting
                          this bug
                          2. Updated class to work with Scintilla v2.01. New messages wrapped include:
                          SCI_SETWRAPINDENTMODE, SCI_GETWRAPINDENTMODE, SCI_INDICSETALPHA, SCI_INDICGETALPHA, SCI_SETEXTRAASCENT, 
                          SCI_GETEXTRAASCENT, SCI_SETEXTRADESCENT, SCI_GETEXTRADESCENT, SCI_MARKERSYMBOLDEFINED, SCI_MARGINSETTEXT, 
                          SCI_MARGINGETTEXT, SCI_MARGINSETSTYLE, SCI_MARGINGETSTYLE, SCI_MARGINSETSTYLES, SCI_MARGINGETSTYLES, 
                          SCI_MARGINTEXTCLEARALL, SCI_MARGINSETSTYLEOFFSET, SCI_MARGINGETSTYLEOFFSET, SCI_ANNOTATIONSETTEXT, 
                          SCI_ANNOTATIONGETTEXT, SCI_ANNOTATIONSETSTYLE, SCI_ANNOTATIONGETSTYLE, SCI_ANNOTATIONSETSTYLES, 
                          SCI_ANNOTATIONGETSTYLES, SCI_ANNOTATIONGETLINES, SCI_ANNOTATIONCLEARALL, SCI_ANNOTATIONSETVISIBLE, 
                          SCI_ANNOTATIONGETVISIBLE, SCI_ANNOTATIONSETSTYLEOFFSET, SCI_ANNOTATIONGETSTYLEOFFSET, 
                          SCI_ADDUNDOACTION, SCI_CHARPOSITIONFROMPOINT, SCI_CHARPOSITIONFROMPOINTCLOSE, SCI_SETMULTIPLESELECTION, 
                          SCI_GETMULTIPLESELECTION, SCI_SETADDITIONALSELECTIONTYPING, SCI_GETADDITIONALSELECTIONTYPING, 
                          SCI_SETADDITIONALCARETSBLINK, SCI_GETADDITIONALCARETSBLINK, SCI_GETSELECTIONS, SCI_CLEARSELECTIONS, 
                          SCI_SETSELECTION, SCI_ADDSELECTION, SCI_SETMAINSELECTION, SCI_GETMAINSELECTION, SCI_SETSELECTIONNCARET, 
                          SCI_GETSELECTIONNCARET, SCI_SETSELECTIONNANCHOR, SCI_GETSELECTIONNANCHOR, SCI_SETSELECTIONNCARETVIRTUALSPACE, 
                          SCI_GETSELECTIONNCARETVIRTUALSPACE, SCI_SETSELECTIONNANCHORVIRTUALSPACE, SCI_GETSELECTIONNANCHORVIRTUALSPACE, 
                          SCI_SETSELECTIONNSTART, SCI_GETSELECTIONNSTART, SCI_SETSELECTIONNEND, SCI_GETSELECTIONNEND, 
                          SCI_SETRECTANGULARSELECTIONCARET, SCI_GETRECTANGULARSELECTIONCARET, SCI_SETRECTANGULARSELECTIONANCHOR, 
                          SCI_GETRECTANGULARSELECTIONANCHOR, SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE, 
                          SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE, 
                          SCI_SETVIRTUALSPACEOPTIONS, SCI_GETVIRTUALSPACEOPTIONS, SCI_SETRECTANGULARSELECTIONMODIFIER, 
                          SCI_GETRECTANGULARSELECTIONMODIFIER, SCI_SETADDITIONALSELFORE, SCI_SETADDITIONALSELBACK, SCI_SETADDITIONALSELALPHA, 
                          SCI_GETADDITIONALSELALPHA, SCI_SETADDITIONALCARETFORE, SCI_GETADDITIONALCARETFORE, SCI_ROTATESELECTION &
                          SCI_SWAPMAINANCHORCARET
         PJN / 22-11-2010 1. Updated copyright details.
                          2. Updated sample app to clean compile on VC 2010
                          3. Updated class to work with Scintilla v2.22. New messages wrapped include:
                          SCI_SETWHITESPACESIZE, SCI_GETWHITESPACESIZE, SCI_SETFONTQUALITY, SCI_GETFONTQUALITY, SCI_SETFIRSTVISIBLELINE, 
                          SCI_SETMULTIPASTE, SCI_GETMULTIPASTE, SCI_GETTAG, SCI_AUTOCGETCURRENTTEXT, SCI_SETADDITIONALCARETSVISIBLE,
                          SCI_GETADDITIONALCARETSVISIBLE, SCI_CHANGELEXERSTATE, SCI_CONTRACTEDFOLDNEXT, SCI_VERTICALCENTRECARET,
                          SCI_GETLEXERLANGUAGE, SCI_PRIVATELEXERCALL, SCI_PROPERTYNAMES, SCI_PROPERTYTYPE, SCI_DESCRIBEPROPERTY, 
                          SCI_DESCRIBEKEYWORDSETS. Also there were some parameter changes to existing messages.
         PJN / 01-04-2011 1. Updated copyright details.
                          2. Updated class to work with Scintilla v2.25. New messages wrapped include:
                          SCI_SETMARGINCURSORN & SCI_GETMARGINCURSORN
         PJN / 09-12-2011 1. Updated class to work with Scintilla v3.0.2. New messages wrapped include: SCI_MARKERSETBACKSELECTED, 
                          SCI_MARKERENABLEHIGHLIGHT, SCI_STYLESETSIZEFRACTIONAL, SCI_STYLEGETSIZEFRACTIONAL, SCI_STYLESETWEIGHT, 
                          SCI_STYLEGETWEIGHT, SCI_COUNTCHARACTERS, SCI_SETEMPTYSELECTION, SCI_CALLTIPSETPOSITION, SCI_GETALLLINESVISIBLE,
                          SCI_BRACEHIGHLIGHTINDICATOR, SCI_BRACEBADLIGHTINDICATOR, SCI_INDICSETOUTLINEALPHA, SCI_INDICGETOUTLINEALPHA,
                          SCI_SETMARGINOPTIONS, SCI_GETMARGINOPTIONS, SCI_MOVESELECTEDLINESUP, SCI_MOVESELECTEDLINESDOWN, SCI_SETIDENTIFIER, 
                          SCI_GETIDENTIFIER, SCI_RGBAIMAGESETWIDTH, SCI_RGBAIMAGESETHEIGHT, SCI_MARKERDEFINERGBAIMAGE, SCI_REGISTERRGBAIMAGE, 
                          SCI_SCROLLTOSTART, SCI_SCROLLTOEND, SCI_SETTECHNOLOGY, SCI_GETTECHNOLOGY & SCI_CREATELOADER
                          Messages dropped include: SCI_SETUSEPALETTE & SCI_GETUSEPALETTE
         PJN / 15-08-2012 1. Updated copyright details
                          2. Updated class to work with Scintilla v3.2.1 New Messaged wrapped include: SCI_DELETERANGE, SCI_GETWORDCHARS,
                          SCI_GETWHITESPACECHARS, SCI_SETPUNCTUATIONCHARS, SCI_GETPUNCTUATIONCHARS, SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR,
                          SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR, SCI_GETRANGEPOINTER, SCI_GETGAPPOSITION, SCI_FINDINDICATORSHOW, 
                          SCI_FINDINDICATORFLASH & SCI_FINDINDICATORHIDE.
                          3. SetDocPointer method now uses a void* parameter instead of an int. This prevents a pointer truncation issue
                          on 64bit platforms. Thanks to Kenny Liu for reporting this issue.
                          4. GetDocPointer method now also returns a void* instead of an int return value
                          5. Updated the code to clean compile on VC 2012
         PJN / 18-01-2013 1. Updated copyright details
                          2. Updated class to work with Scintilla v3.2.4. New messages wrapped include: SCI_GETSELECTIONEMPTY,
                          SCI_RGBAIMAGESETSCALE, SCI_VCHOMEDISPLAY, SCI_VCHOMEDISPLAYEXTEND, SCI_GETCARETLINEVISIBLEALWAYS & 
                          SCI_SETCARETLINEVISIBLEALWAYS.
                          3. The classes are now encapsulated in a Scintilla namespace if the SCI_NAMESPACE define
                          is defined. This is consistent with how the scintilla.h header file operates in the 
                          presence of this define. Thanks to Markus Nissl for prompting this update.
                          4. Updated the sample app to compile when the SCI_NAMESPACE define is defined.
                          5. The sample app is now built by default with the SCI_NAMESPACE defined. This means that all the 
                          classes of the author will appear in the "Scintilla" namespace.
                          6. The demo app now loads the SciLexer.dll from the application directory only. This avoids DLL planting security 
                          issues.
         PJN / 23-02-2013 1. PrivateLexerCall method now uses a void* parameter instead of an int. This prevents a pointer truncation issue
                          on 64bit platforms. Thanks to Simon Smith for reporting this issue.
         PJN / 18-01-2013 1. Updated class to work with Scintilla v3.3.3. New messages wrapped include: SCI_SCROLLRANGE, SCI_FOLDLINE, 
                          SCI_FOLDCHILDREN, SCI_EXPANDCHILDREN, SCI_FOLDALL, SCI_SETAUTOMATICFOLD, SCI_GETAUTOMATICFOLD, SCI_AUTOCSETORDER,
                          SCI_AUTOCGETORDER, SCI_RELEASEALLEXTENDEDSTYLES, SCI_ALLOCATEEXTENDEDSTYLES, SCI_SETLINEENDTYPESALLOWED, 
                          SCI_GETLINEENDTYPESALLOWED, SCI_GETLINEENDTYPESACTIVE, SCI_GETLINEENDTYPESSUPPORTED, SCI_ALLOCATESUBSTYLES,
                          SCI_GETSUBSTYLESSTART, SCI_GETSUBSTYLESLENGTH, SCI_FREESUBSTYLES, SCI_SETIDENTIFIERS, SCI_DISTANCETOSECONDARYSTYLES &
                          SCI_GETSUBSTYLEBASES.
                          2. Updated all the MFC MESSAGE_MAP's to use modern C++ style to reference methods of a class.
                          3. Fixed a heap overwrite bug in the two versions of the GetSelText(BOOL bDirect) method. The code now correctly uses
                          SCI_GETSELTEXT(0,0) to determine the buffer size to retrieve the data into. Thanks to Bengt Vagnhammar for reporting
                          this bug.
         PJN / 26-01-2015 1. Updated copyright details
                          2. Updated the code to clean compile on VC 2013
                          3. Updated class to work with Scintilla v3.5.3. New messages wrapped include: SCI_CHANGEINSERTION,
                          SCI_CLEARTABSTOPS, SCI_ADDTABSTOP, SCI_GETNEXTTABSTOP, SCI_GETIMEINTERACTION, SCI_SETIMEINTERACTION, 
                          SCI_CALLTIPSETPOSSTART, SCI_GETPHASESDRAW, SCI_SETPHASESDRAW, SCI_POSITIONRELATIVE, SCI_AUTOCSETMULTI, 
                          SCI_AUTOCGETMULTI, SCI_SETMOUSESELECTIONRECTANGULARSWITCH, SCI_GETMOUSESELECTIONRECTANGULARSWITCH, 
                          SCI_DROPSELECTIONN, SCI_SETREPRESENTATION, SCI_GETREPRESENTATION, SCI_CLEARREPRESENTATION, 
                          SCI_GETSTYLEFROMSUBSTYLE & SCI_GETPRIMARYSTYLEFROMSTYLE 
         PJN / 19-09-2015 1. Updated class to work with Scintilla v3.6.1. New messages wrapped include: SCI_INDICSETHOVERSTYLE, 
                          SCI_INDICGETHOVERSTYLE, SCI_INDICSETHOVERFORE, SCI_INDICGETHOVERFORE, SCI_INDICSETFLAGS, 
                          SCI_INDICGETFLAGS, SCI_SETTARGETRANGE, SCI_GETTARGETTEXT, SCI_TARGETWHOLEDOCUMENT, SCI_ISRANGEWORD.
                          SCI_MULTIPLESELECTADDNEXT & SCI_MULTIPLESELECTADDEACH. 
                          Removed messages include: SCI_SETKEYSUNICODE & SCI_GETKEYSUNICODE
                          2. All APIs which use a logical document position which previously used a C long has now been replaced with the 
                          Scintilla define "Sci_Position". This is to mirror the ongoing changes in Scintilla to enable support for documents
                          larger than 2GB.
         PJN / 23-01-2016 1. Updated copyright details.
                          2. Updated class to work with Scintilla v3.6.3. New messages wrapped include: SCI_SETIDLESTYLING &
                          SCI_GETIDLESTYLING
         PJN / 11-07-2016 1. Verified class against Scintilla v3.6.6. As no new messages were introduced between v3.6.3 and v3.6.6 no
                          changes were required in the code.
                          2. Removed the bDirect parameter from all the method calls and instead replaced this functionality with a 
                          new pair of getter / setter methods called GetCallDirect and SetCallDirect. Thanks to Chad Marlow for prompting this
                          update
         PJN / 25-07-2016 1. Added SAL annotations to all the code
         PJN / 16-10-2016 1. Replaced all occurrences of NULL with nullptr throughout the codebase. This now means that the minimum 
                          requirement to compile the code is Visual Studio 2010 or later. Thanks to Markus Nissl for requesting this update.
                          2. Updated class to work with Scintilla v3.7.0. New messages wrapped include: SCI_SETMARGINBACKN, 
                          SCI_GETMARGINBACKN, SCI_SETMARGINS, SCI_GETMARGINS, SCI_MULTIEDGEADDLINE, SCI_MULTIEDGECLEARALL, 
                          SCI_SETMOUSEWHEELCAPTURES & SCI_GETMOUSEWHEELCAPTURES.
         PJN / 20-12-2016 1. Updated class to work with Scintilla v3.7.1. New messages wrapped include: SCI_GETTABDRAWMODE, SCI_SETTABDRAWMODE, 
                          SCI_TOGGLEFOLDSHOWTEXT & SCI_FOLDDISPLAYTEXTSETSTYLE. The parameter to support the SCI_USEPOPUP message has been 
                          changed from a BOOL to an int.
                          2. Updated code to use Sci_RangeToFormat typedef instead of RangeToFormat
                          3. Updated code to use Sci_TextToFind typedef instead of TextToFind
                          4. Updated code to no longer use Scintilla namespace which has been removed from 
                          Scintilla.h
         PJN / 04-03-2017 1. Updated copyright details
                          2. Updated class to work with Scintilla v3.7.3. The only change to support this version was to have now no return 
                          value from the SetSelection and AddSelection methods
                          3. Updated the download to include the correct VC 2010 project files. Thanks to Kenny Lau for reporting this
                          issue.
         PJN / 03-04-2017 1. Updated class to work with Scintilla v3.7.4. New messages wrapped include: SCI_SETACCESSIBILITY & 
                          SCI_GETACCESSIBILITY
         PJN / 12-06-2017 1. Updated class to work with Scintilla v3.7.5. New messages wrapped include: SCI_GETCARETLINEFRAME, 
                          SCI_SETCARETLINEFRAME & SCI_LINEREVERSE
         PJN / 31-08-2017 1. Updated class to work with Scintilla v4.0.0. New messages wrapped include: SCI_GETNAMEDSTYLES, SCI_NAMEOFSTYLE,
                          SCI_TAGSOFSTYLE & SCI_DESCRIPTIONOFSTYLE. Messages removed include SCI_GETTWOPHASEDRAW & SCI_SETTWOPHASEDRAW
                          2. Fixed up a number of compiler warnings when the code is compiled for x64
         PJN / 27-12-2017 1. Updated class to work with Scintilla v4.0.2. Some messages have been removed in 4.0.2 including SCI_SETSTYLEBITS,
                          SCI_GETSTYLEBITS & SCI_GETSTYLEBITSNEEDED.
         PJN / 03-01-2018 1. Updated copyright details.
                          2. Removed Unicode versions of MarginSetStyles & AnnotationSetStyles methods as these methods take byte buffers
                          and do not take text data. Thanks to Karagoez Yusuf for reporting this issue.
         PJN / 18-03-2018 1. Updated class to work with Scintilla v4.0.3. New parameters to SCI_CREATEDOCUMENT & SCI_CREATELOADER messages.
                          New messages wrapped include: SCI_GETMOVEEXTENDSSELECTION message, SCI_GETBIDIRECTIONAL & SCI_SETBIDIRECTIONAL
                          2. SCI_ADDREFDOCUMENT and SCI_RELEASEDOCUMENT wrappers now use void* for the document parameter.
         PJN / 03-05-2018 1. Verified the code works with the latest Scintilla v4.0.4. No new messages were added for this release of
                          scintilla.
         PJN / 14-07-2018 1. Fixed a number of C++ core guidelines compiler warnings. These changes mean that
                          the code will now only compile on VC 2017 or later.
                          2. Code page is now explicitly set to ANSI when building for ANSI. This is necessary because as of Scintilla 4 the
                          default code page is now UTF-8. Thanks to Karagoez Yusuf for reporting this issue.
                          3. CScintillaCtrl::SetProperty has been renamed SetScintillaProperty to avoid clashing with CWnd::SetProperty
                          4. CScintillaCtrl::GetProperty has been renamed GetScintillaProperty to avoid clashing with CWnd::GetProperty
                          5. Updated class to work with Scintilla v4.1.0. New messages wrapped include: SCI_GETDOCUMENTOPTIONS
         PJN / 09-09-2018 1. Fixed a number of compiler warnings when using VS 2017 15.8.2
                          2. Updated class to work with Scintilla v4.1.1. New messages wrapped include: SCI_COUNTCODEUNITS,
                          SCI_POSITIONRELATIVECODEUNITS, SCI_GETLINECHARACTERINDEX, SCI_ALLOCATELINECHARACTERINDEX, 
                          SCI_RELEASELINECHARACTERINDEX, SCI_LINEFROMINDEXPOSITION & SCI_INDEXPOSITIONFROMLINE
         PJN / 19-01-2019 1. Updated copyright details
                          2. Updated class to work with Scintilla v4.1.3. New messages wrapped include: SCI_SETCOMMANDEVENTS & 
                          SCI_GETCOMMANDEVENTS.
                          3. Added code to suppress C4263 off by default compiler warning. Thanks to Karagoez Yusuf for reporting this issue.
         PJN / 23-02-2019 1. Fixed a number of compiler warnings when the code is compiled with VS 2019 Preview
         PJN / 02-04-2019 1. Verified the code works with the latest Scintilla v4.1.4. No new messages were added for this release of
                          scintilla.
         PJN / 25-06-2019 1. Updated class to work with Scintilla v4.1.7. New messages wrapped include: SCI_SETCHARACTERCATEGORYOPTIMIZATION,
                          SCI_GETCHARACTERCATEGORYOPTIMIZATION, SCI_FOLDDISPLAYTEXTGETSTYLE, SCI_SETDEFAULTFOLDDISPLAYTEXT &
                          SCI_GETDEFAULTFOLDDISPLAYTEXT.
         PJN / 23-08-2019 1. Updated class to work with Scintilla v4.2.0. Various API definitions have been updated to use Sci_Position 
                          instead of int parameters. No new actual messages were added.
         PJN / 03-11-2019 1. Updated class to work with Scintilla v4.2.1. New messages wrapper include: SCI_SETTABMINIMUMWIDTH and
                          SCI_GETTABMINIMUMWIDTH.
                          2. Updated initialization of various structs to use C++ 11 list initialization
         PJN / 27-12-2019 1. Updated class to work with Scintilla v4.2.3. New messages wrapped include: SCI_SETTARGETSTARTVIRTUALSPACE, 
                          SCI_GETTARGETSTARTVIRTUALSPACE, SCI_SETTARGETENDVIRTUALSPACE, SCI_GETTARGETENDVIRTUALSPACE, 
                          SCI_GETSELECTIONNSTARTVIRTUALSPACE & SCI_GETSELECTIONNENDVIRTUALSPACE.
                          2. Fixed various Clang-Tidy static code analysis warnings in the code.
         PJN / 21-03-2020 1. Updated copyright details.
                          2. Fixed more Clang-Tidy static code analysis warnings in the code.
                          3. Updated class to work with Scintilla v4.3.2. New messages wrapped include: SCI_SETILEXER
         PJN / 07-05-2020 1. Added missing static_casts in the GetSelectionNStartVirtualSpace and 
                          GetSelectionNEndVirtualSpace methods. Thanks to Yusuf Karagöz for reporting this issue.
                          2. Updated class to work with Scintilla v4.3.3. New messages wrapped include: SCI_MARKERHANDLEFROMLINE and 
                          SCI_MARKERNUMBERFROMLINE.
                          3. Changed two parameters to CallTipSetHlt method to be Sci_Position from int.
                          4. Changed return value from IndicatorStart method to be Sci_Position from int.
                          5. Changed return value from IndicatorEnd method to be Sci_Position from int.
         PJN / 14-06-2020 1. Verified the code against Scintilla v4.4.3.
         PJN / 15-08-2020 1. Updated class to work with Scintilla v4.4.4. New messages wrapped include: SCI_BRACEMATCHNEXT,
                          SCI_EOLANNOTATIONSETTEXT, SCI_EOLANNOTATIONGETTEXT, SCI_EOLANNOTATIONSETSTYLE, SCI_EOLANNOTATIONGETSTYLE, 
                          SCI_EOLANNOTATIONCLEARALL, SCI_EOLANNOTATIONSETVISIBLE, SCI_EOLANNOTATIONGETVISIBLE, 
                          SCI_EOLANNOTATIONSETSTYLEOFFSET & SCI_EOLANNOTATIONGETSTYLEOFFSET.
         PJN / 26-09-2020 1. Updated class to work with Scintilla v4.4.5. New messages wrapped include: SCI_GETMULTIEDGECOLUMN
         PJN / 10-05-2021 1. Updated class to work with Scintilla v5.0.2. New messages wrapped include: SCI_SETFONTLOCALE, 
                          SCI_GETFONTLOCALE, SCI_MARKERSETFORETRANSLUCENT, SCI_MARKERSETBACKTRANSLUCENT, SCI_MARKERSETBACKSELECTEDTRANSLUCENT,
                          SCI_MARKERSETSTROKEWIDTH, SCI_SETELEMENTCOLOUR, SCI_GETELEMENTCOLOUR, SCI_RESETELEMENTCOLOUR, SCI_GETELEMENTISSET, 
                          SCI_GETELEMENTALLOWSTRANSLUCENT, SCI_INDICSETSTROKEWIDTH, SCI_INDICGETSTROKEWIDTH & SCI_SUPPORTSFEATURE
                          2. Updated copyright details
         PJN / 05-06-2021 1. Updated class to work with Scintilla v5.0.3. New messages wrapped include: SCI_MARKERGETLAYER, 
                          SCI_MARKERSETLAYER, SCI_GETELEMENTBASECOLOUR, SCI_GETSELECTIONLAYER, SCI_SETSELECTIONLAYER, SCI_GETCARETLINELAYER
                          & SCI_SETCARETLINELAYER.
         PJN / 11-07-2021 1. Updated class to work with Scintilla v5.1.0. New messages wrapped include: SCI_GETDIRECTSTATUSFUNCTION,
                          SCI_REPLACERECTANGULAR, SCI_CLEARALLREPRESENTATIONS, SCI_SETREPRESENTATIONAPPEARANCE,
                          SCI_GETREPRESENTATIONAPPEARANCE, SCI_SETREPRESENTATIONCOLOUR & SCI_GETREPRESENTATIONCOLOUR
                          2. Changed the return values from GetDirectFunction & GetDirectPointer to less generic data types.
         PJN / 14-08-2021 1. Updated class to work with Scintilla v5.1.1. New messages wrapped include: SCI_AUTOCSETOPTIONS, 
                          SCI_AUTOCGETOPTIONS & SCI_ALLOCATELINES.
         PJN / 30-09-2021 1. Updated class to work with Scintilla v5.1.3. New messages wrapped include: 
                          SCI_STYLESETCHECKMONOSPACED, SCI_STYLEGETCHECKMONOSPACED, SCI_GETCARETLINEHIGHLIGHTSUBLINE & 
                          SCI_SETCARETLINEHIGHLIGHTSUBLINE.
         PJN / 31-03-2022 1. Updated copyright details.
                          2. Updated class to work with Scintilla v5.2.2. New messages wrapped include: 
                          SCI_GETSTYLEINDEXAT, SCI_SETLAYOUTTHREADS and SCI_GETLAYOUTTHREADS.
                          3. Updated the code to use C++ uniform initialization for all variable declarations.
         PJN / 31-03-2022 1. Updated class to work with Scintilla v5.2.4. New messages wrapped include:
                          SCI_FINDTEXTFULL, SCI_FORMATRANGEFULL & SCI_GETTEXTRANGEFULL.
         PJN / 10-08-2022 1. Updated all line parameters to use intptr_t instead of int. Thanks to Markus Nissl for reporting this issue.

Copyright (c) 2004 - 2022 by PJ Naughter (Web: www.naughter.com, Email: pjna@naughter.com)

All rights reserved.

Copyright / Usage Details:

You are allowed to include the source code in any product (commercial, shareware, freeware or otherwise) 
when your product is released in binary form. You are allowed to modify the source code in any way you want 
except you cannot modify the copyright details at the top of each module. If you want to distribute source 
code with your application, then you are only allowed to distribute versions released by the author. This is 
to maintain a single distribution point for the source code.

*/


//////////////////// Includes /////////////////////////////////////////////////

#include "pch.h"
#include "ScintillaCtrl.h"


//////////////////// Statics / Macros /////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif //#ifdef _DEBUG


//////////////////// Implementation ///////////////////////////////////////////

#pragma warning(suppress: 26433 26440 26477)
IMPLEMENT_DYNAMIC(CScintillaCtrl, CWnd)

CScintillaCtrl::CScintillaCtrl() noexcept : m_bCallDirect{TRUE},
                                            m_DirectFunction{nullptr},
                                            m_DirectStatusFunction{nullptr},
                                            m_DirectPointer{0}
{
}

BOOL CScintillaCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwExStyle, LPVOID lpParam)
{
  //Call our base class implementation of CWnd::CreateEx
  if (!CreateEx(dwExStyle, _T("scintilla"), nullptr, dwStyle, rect, pParentWnd, nID, lpParam))
    return FALSE;

  //Setup the direct access data
  SetupDirectAccess();

  //If we are running as Unicode, then use the UTF8 codepage else use the ANSI codepage
#ifdef _UNICODE
  SetCodePage(SC_CP_UTF8);
#else
  SetCodePage(0);
#endif //#ifdef _UNICODE

  return TRUE;
}

void CScintillaCtrl::SetupDirectAccess()
{
  //Setup the direct access data
  m_DirectFunction = GetDirectFunction();
  m_DirectPointer = GetDirectPointer();
  m_DirectStatusFunction = GetDirectStatusFunction();
}

BOOL CScintillaCtrl::GetCallDirect() const noexcept
{
  return m_bCallDirect;
}

void CScintillaCtrl::SetCallDirect(_In_ BOOL bDirect) noexcept
{
  m_bCallDirect = bDirect;
}

SciFnDirect CScintillaCtrl::GetDirectFunction()
{
#pragma warning(suppress: 26490)
  return reinterpret_cast<SciFnDirect>(SendMessage(SCI_GETDIRECTFUNCTION, 0, 0));
}

sptr_t CScintillaCtrl::GetDirectPointer()
{
  return SendMessage(SCI_GETDIRECTPOINTER, 0, 0);
}

#ifdef _UNICODE
CStringA CScintillaCtrl::W2UTF8(_In_NLS_string_(nLength) const wchar_t* pszText, _In_ int nLength)
{
  //First call the function to determine how much space we need to allocate
  int nUTF8Length{WideCharToMultiByte(CP_UTF8, 0, pszText, nLength, nullptr, 0, nullptr, nullptr)};

  //If the calculated length is zero, then ensure we have at least room for a null terminator
  if (nUTF8Length == 0)
    nUTF8Length = 1;

  //Now recall with the buffer to get the converted text
  CStringA sUTF;
#pragma warning(suppress: 26429)
  char* const pszUTF8Text{sUTF.GetBuffer(nUTF8Length + 1)}; //include an extra byte because we may be null terminating the string ourselves
  int nCharsWritten{WideCharToMultiByte(CP_UTF8, 0, pszText, nLength, pszUTF8Text, nUTF8Length, nullptr, nullptr)};

  //Ensure we null terminate the text if WideCharToMultiByte doesn't do it for us
  if (nLength != -1)
  {
#pragma warning(suppress: 26496)
    AFXASSUME(nCharsWritten <= nUTF8Length);
#pragma warning(suppress: 26481)
    pszUTF8Text[nCharsWritten] = '\0';
  }
  sUTF.ReleaseBuffer();

  return sUTF;
}

CStringW CScintillaCtrl::UTF82W(_In_NLS_string_(nLength) const char* pszText, _In_ int nLength)
{
  //First call the function to determine how much space we need to allocate
  int nWideLength{MultiByteToWideChar(CP_UTF8, 0, pszText, nLength, nullptr, 0)};

  //If the calculated length is zero, then ensure we have at least room for a null terminator
  if (nWideLength == 0)
    nWideLength = 1;

  //Now recall with the buffer to get the converted text
  CStringW sWideString;
#pragma warning(suppress: 26429)
  wchar_t* pszWText{sWideString.GetBuffer(nWideLength + 1)}; //include an extra byte because we may be null terminating the string ourselves
  int nCharsWritten{MultiByteToWideChar(CP_UTF8, 0, pszText, nLength, pszWText, nWideLength)};

  //Ensure we null terminate the text if MultiByteToWideChar doesn't do it for us
  if (nLength != -1)
  {
#pragma warning(suppress: 26496)
    AFXASSUME(nCharsWritten <= nWideLength);
#pragma warning(suppress: 26481)
    pszWText[nCharsWritten] = '\0';
  }
  sWideString.ReleaseBuffer();

  return sWideString;
}

void CScintillaCtrl::AddText(_In_ int length, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, length)};

  //Call the native scintilla version of the function with the UTF8 text
  AddText(sUTF8.GetLength(), sUTF8);
}

void CScintillaCtrl::InsertText(_In_ Sci_Position pos, _In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  InsertText(pos, sUTF8);
}

void CScintillaCtrl::ChangeInsertion(_In_ int length, _In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  ChangeInsertion(length, sUTF8);
}

CStringW CScintillaCtrl::GetSelText()
{
  //Work out the length of string to allocate
  const Sci_Position nUTF8Length{GetSelText(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
#pragma warning(suppress: 26472)
  GetSelText(sUTF8.GetBufferSetLength(static_cast<int>(nUTF8Length)));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetCurLine()
{
  //Work out the length of string to allocate
  const Sci_Position nUTF8Length{GetCurLine(0, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
#pragma warning(suppress: 26472)
  GetCurLine(nUTF8Length, sUTF8.GetBufferSetLength(static_cast<int>(nUTF8Length)));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

void CScintillaCtrl::StyleSetFont(_In_ int style, _In_z_ const wchar_t* fontName)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(fontName, -1)};

  StyleSetFont(style, sUTF8);
}

void CScintillaCtrl::SetWordChars(_In_z_ const wchar_t* characters)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(characters, -1)};

  SetWordChars(sUTF8);
}

void CScintillaCtrl::AutoCShow(_In_ int lenEntered, _In_z_ const wchar_t* itemList)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(itemList, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  AutoCShow(lenEntered, sUTF8);
}

void CScintillaCtrl::AutoCStops(_In_z_ const wchar_t* characterSet)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(characterSet, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  AutoCStops(sUTF8);
}

void CScintillaCtrl::AutoCSelect(_In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  AutoCSelect(sUTF8);
}

void CScintillaCtrl::AutoCSetFillUps(_In_z_ const wchar_t* characterSet)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(characterSet, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  AutoCSetFillUps(sUTF8);
}

void CScintillaCtrl::UserListShow(_In_ int listType, _In_z_ const wchar_t* itemList)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(itemList, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  UserListShow(listType, sUTF8);
}

CStringW CScintillaCtrl::GetLine(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const Sci_Position nUTF8Length{GetLine(line, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
#pragma warning(suppress: 26472)
  GetLine(line, sUTF8.GetBufferSetLength(static_cast<int>(nUTF8Length)));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

void CScintillaCtrl::ReplaceSel(_In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  ReplaceSel(sUTF8);
}

void CScintillaCtrl::SetText(_In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  SetText(sUTF8);
}

CStringW CScintillaCtrl::GetText(_In_ int length)
{
  //Work out the length of string to allocate
  const int nUTF8Length{length * 4}; //A Unicode character can take up to 4 octets when expressed as UTF8

  //Call the function which does the work
  CStringA sUTF8;
  GetText(nUTF8Length, sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  CStringW sWideText{UTF82W(sUTF8, -1)};

  return sWideText.Left(length-1);
}

Sci_Position CScintillaCtrl::ReplaceTarget(_In_ Sci_Position length, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
#pragma warning(suppress: 26472)
  CStringA sUTF8{W2UTF8(text, static_cast<int>(length))};

  //Call the native scintilla version of the function with the UTF8 text
  return ReplaceTarget(sUTF8.GetLength(), sUTF8);
}

Sci_Position CScintillaCtrl::ReplaceTargetRE(_In_ Sci_Position length, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
#pragma warning(suppress: 26472)
  CStringA sUTF8{W2UTF8(text, static_cast<int>(length))};

  //Call the native scintilla version of the function with the UTF8 text
  return ReplaceTargetRE(sUTF8.GetLength(), sUTF8);
}

Sci_Position CScintillaCtrl::SearchInTarget(_In_ Sci_Position length, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
#pragma warning(suppress: 26472)
  CStringA sUTF8{W2UTF8(text, static_cast<int>(length))};

  //Call the native scintilla version of the function with the UTF8 text
  return SearchInTarget(sUTF8.GetLength(), sUTF8);
}

void CScintillaCtrl::CallTipShow(_In_ Sci_Position pos, _In_z_ const wchar_t* definition)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(definition, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  CallTipShow(pos, sUTF8);
}

int CScintillaCtrl::TextWidth(_In_ int style, _In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  return TextWidth(style, sUTF8);
}

void CScintillaCtrl::AppendText(_In_ int length, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, length)};

  //Call the native scintilla version of the function with the UTF8 text
  AppendText(sUTF8.GetLength(), sUTF8);
}

Sci_Position CScintillaCtrl::SearchNext(_In_ int flags, _In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  return SearchNext(flags, sUTF8);
}

Sci_Position CScintillaCtrl::SearchPrev(_In_ int flags, _In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  return SearchPrev(flags, sUTF8);
}

void CScintillaCtrl::CopyText(_In_ int length, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, length)};

  //Call the native scintilla version of the function with the UTF8 text
  CopyText(sUTF8.GetLength(), sUTF8);
}

void CScintillaCtrl::SetWhitespaceChars(_In_z_ const wchar_t* characters)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(characters, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  SetWhitespaceChars(sUTF8);
}

void CScintillaCtrl::SetScintillaProperty(_In_z_ const wchar_t* key, _In_z_ const wchar_t* value)
{
  //Convert the unicode texts to UTF8
  CStringA sUTF8Key{W2UTF8(key, -1)};
  CStringA sUTF8Value{W2UTF8(value, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  SetScintillaProperty(sUTF8Key, sUTF8Value);
}

void CScintillaCtrl::SetKeyWords(_In_ int keywordSet, _In_z_ const wchar_t* keyWords)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(keyWords, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  SetKeyWords(keywordSet, sUTF8);
}

void CScintillaCtrl::SetIdentifiers(_In_ int style, _In_z_ const wchar_t* identifiers)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(identifiers, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  SetIdentifiers(style, sUTF8);
}

CStringW CScintillaCtrl::GetScintillaProperty(_In_z_ const wchar_t* key)
{
  //Validate our parameters
  ASSERT(key != nullptr);

  //Convert the Key value to UTF8
  CStringA sUTF8Key{W2UTF8(key, -1)};

  //Work out the length of string to allocate
  const int nUTF8ValueLength{GetScintillaProperty(sUTF8Key, nullptr)};

  //Call the function which does the work
  CStringA sUTF8Value;
  GetScintillaProperty(sUTF8Key, sUTF8Value.GetBufferSetLength(nUTF8ValueLength));
  sUTF8Value.ReleaseBuffer();

  return UTF82W(sUTF8Value, -1);
}

CStringW CScintillaCtrl::GetPropertyExpanded(_In_z_ const wchar_t* key)
{
  //Validate our parameters
  ASSERT(key != nullptr);

  //Convert the Key value to UTF8
  CStringA sUTF8Key{W2UTF8(key, -1)};

  //Work out the length of string to allocate
  const int nUTF8ValueLength{GetPropertyExpanded(sUTF8Key, nullptr)};

  //Call the function which does the work
  CStringA sUTF8Value;
  GetPropertyExpanded(sUTF8Key, sUTF8Value.GetBufferSetLength(nUTF8ValueLength));
  sUTF8Value.ReleaseBuffer();

  return UTF82W(sUTF8Value, -1);
}

int CScintillaCtrl::GetPropertyInt(_In_z_ const wchar_t* key, _In_ int defaultValue)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(key, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  return GetPropertyInt(sUTF8, defaultValue);
}

CStringW CScintillaCtrl::StyleGetFont(_In_ int style)
{
  //Work out the length of string to allocate
  const int nUTF8Length{StyleGetFont(style, nullptr)};

  //Call the function which does the work
  CStringA sUTF8FontName;
  StyleGetFont(style, sUTF8FontName.GetBufferSetLength(nUTF8Length));
  sUTF8FontName.ReleaseBuffer();
  return UTF82W(sUTF8FontName, -1);
}

void CScintillaCtrl::MarginSetText(_In_ intptr_t line, _In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  MarginSetText(line, sUTF8);
}

void CScintillaCtrl::AnnotationSetText(_In_ intptr_t line, _In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  AnnotationSetText(line, sUTF8);
}

CStringW CScintillaCtrl::AutoCGetCurrentText()
{
  //Work out the length of string to allocate
  const int nUTF8Length{AutoCGetCurrentText(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  AutoCGetCurrentText(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetLexerLanguage()
{
  //Work out the length of string to allocate
  const int nUTF8Length{GetLexerLanguage(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetLexerLanguage(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::PropertyNames()
{
  //Work out the length of string to allocate
  const int nUTF8Length{PropertyNames(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  PropertyNames(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

int CScintillaCtrl::PropertyType(_In_z_ const wchar_t* name)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(name, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  return PropertyType(sUTF8);
}

void CScintillaCtrl::ToggleFoldShowText(_In_ intptr_t line, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  ToggleFoldShowText(line, sUTF8);
}

void CScintillaCtrl::SetDefaultFoldDisplayText(_In_z_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  SetDefaultFoldDisplayText(sUTF8);
}

void CScintillaCtrl::EOLAnnotationSetText(_In_ intptr_t line, _In_ const wchar_t* text)
{
  //Convert the unicode text to UTF8
  CStringA sUTF8{W2UTF8(text, -1)};

  //Call the native scintilla version of the function with the UTF8 text
  EOLAnnotationSetText(line, sUTF8);
}

CStringW CScintillaCtrl::EOLAnnotationGetText(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nUTF8Length{EOLAnnotationGetText(line, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  EOLAnnotationGetText(line, sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::DescribeProperty(_In_z_ const wchar_t* name)
{
  //Convert the name value to UTF8
  CStringA sUTF8KName{W2UTF8(name, -1)};

  //Work out the length of string to allocate
  const int nUTF8Length{DescribeProperty(sUTF8KName, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  DescribeProperty(sUTF8KName, sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::DescribeKeyWordSets()
{
  //Work out the length of string to allocate
  const int nUTF8Length{DescribeKeyWordSets(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  DescribeKeyWordSets(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetTag(_In_ int tagNumber)
{
  //Work out the length of string to allocate
  const int nUTF8Length{GetTag(tagNumber, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetTag(tagNumber, sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetWordChars()
{
  //Work out the length of string to allocate
  const int nValueLength{GetWordChars(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetWordChars(sUTF8.GetBufferSetLength(nValueLength));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetWhitespaceChars()
{
  //Work out the length of string to allocate
  const int nValueLength{GetWhitespaceChars(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetWhitespaceChars(sUTF8.GetBufferSetLength(nValueLength));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetPunctuationChars()
{
  //Work out the length of string to allocate
  const int nValueLength{GetPunctuationChars(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetPunctuationChars(sUTF8.GetBufferSetLength(nValueLength));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetTargetText()
{
  //Work out the length of string to allocate
  const Sci_Position nValueLength{GetTargetText(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
#pragma warning(suppress: 26472)
  GetTargetText(sUTF8.GetBufferSetLength(static_cast<int>(nValueLength)));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::NameOfStyle(_In_ int style)
{
  //Work out the length of string to allocate
  const int nUTF8ValueLength{NameOfStyle(style, nullptr)};

  //Call the function which does the work
  CStringA sUTF8Value;
  NameOfStyle(style, sUTF8Value.GetBufferSetLength(nUTF8ValueLength));
  sUTF8Value.ReleaseBuffer();

  return UTF82W(sUTF8Value, -1);
}

CStringW CScintillaCtrl::TagsOfStyle(_In_ int style)
{
  //Work out the length of string to allocate
  const int nUTF8ValueLength{TagsOfStyle(style, nullptr)};

  //Call the function which does the work
  CStringA sUTF8Value;
  TagsOfStyle(style, sUTF8Value.GetBufferSetLength(nUTF8ValueLength));
  sUTF8Value.ReleaseBuffer();

  return UTF82W(sUTF8Value, -1);
}

CStringW CScintillaCtrl::DescriptionOfStyle(_In_ int style)
{
  //Work out the length of string to allocate
  const int nUTF8ValueLength{DescriptionOfStyle(style, nullptr)};

  //Call the function which does the work
  CStringA sUTF8Value;
  DescriptionOfStyle(style, sUTF8Value.GetBufferSetLength(nUTF8ValueLength));
  sUTF8Value.ReleaseBuffer();

  return UTF82W(sUTF8Value, -1);
}

CStringW CScintillaCtrl::GetDefaultFoldDisplayText()
{
  //Work out the length of string to allocate
  const int nUTF8Length{GetDefaultFoldDisplayText(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetDefaultFoldDisplayText(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::MarginGetText(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nUTF8Length{MarginGetText(line, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetDefaultFoldDisplayText(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetFontLocale()
{
  //Work out the length of string to allocate
  const int nUTF8Length{GetFontLocale(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetFontLocale(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::MarginGetStyles(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nUTF8Length{MarginGetStyles(line, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  MarginGetStyles(line, sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::AnnotationGetText(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nUTF8Length{AnnotationGetText(line, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  AnnotationGetText(line, sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::AnnotationGetStyles(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nUTF8Length{AnnotationGetStyles(line, nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  AnnotationGetStyles(line, sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}

CStringW CScintillaCtrl::GetSubStyleBases()
{
  //Work out the length of string to allocate
  const int nUTF8Length{GetSubStyleBases(nullptr)};

  //Call the function which does the work
  CStringA sUTF8;
  GetSubStyleBases(sUTF8.GetBufferSetLength(nUTF8Length));
  sUTF8.ReleaseBuffer();

  //Now convert the UTF8 text back to Unicode
  return UTF82W(sUTF8, -1);
}


#else

CStringA CScintillaCtrl::GetSelText()
{
  //Work out the length of string to allocate
  const Sci_Position nLength{GetSelText(nullptr)};

  //Call the function which does the work
  CStringA sSelText;
#pragma warning(suppress: 26472)
  GetSelText(sSelText.GetBufferSetLength(static_cast<int>(nLength)));
  sSelText.ReleaseBuffer();
  return sSelText;
}

CStringA CScintillaCtrl::GetCurLine()
{
  //Call the function which does the work
  CStringA sCurLine;
  const Sci_Position nLength{GetCurLine(0, nullptr)};
#pragma warning(suppress: 26472)
  char* pszCurLine{sCurLine.GetBufferSetLength(static_cast<int>(nLength))};
  GetCurLine(nLength, pszCurLine);
  sCurLine.ReleaseBuffer();
  return sCurLine;
}

CStringA CScintillaCtrl::GetLine(_In_ intptr_t line)
{
  //Call the function which does the work
  CStringA sLine;
  const Sci_Position nLength{GetLine(line, nullptr)};
#pragma warning(suppress: 26472)
  char* pszLine{sLine.GetBufferSetLength(static_cast<int>(nLength))};
  GetLine(line, pszLine);
  sLine.ReleaseBuffer();
  return sLine;
}

CStringA CScintillaCtrl::GetScintillaProperty(_In_z_ const char* key)
{
  //Validate our parameters
  ASSERT(key != nullptr);

  //Work out the length of string to allocate
  const int nValueLength{GetScintillaProperty(key, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  GetScintillaProperty(key, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::GetText(_In_ int length)
{
  //Call the function which does the work
  CString sText;
  GetText(length, sText.GetBufferSetLength(length));
  sText.ReleaseBuffer();

  return sText;
}

CStringA CScintillaCtrl::GetPropertyExpanded(_In_z_ const char* key)
{
  //Validate our parameters
  ASSERT(key != nullptr);

  //Work out the length of string to allocate
  const int nValueLength{GetPropertyExpanded(key, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  GetPropertyExpanded(key, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::StyleGetFont(_In_ int style)
{
  //Work out the length of string to allocate
  const int nValueLength{StyleGetFont(style, nullptr)};

  //Call the function which does the work
  CStringA sFontName;
  StyleGetFont(style, sFontName.GetBufferSetLength(nValueLength));
  sFontName.ReleaseBuffer();

  return sFontName;
}

CStringA CScintillaCtrl::AutoCGetCurrentText()
{
  //Work out the length of string to allocate
  const int nLength{AutoCGetCurrentText(nullptr)};

  //Call the function which does the work
  CStringA sText;
  AutoCGetCurrentText(sText.GetBufferSetLength(nLength));
  sText.ReleaseBuffer();

  return sText;
}

CStringA CScintillaCtrl::GetLexerLanguage()
{
  //Work out the length of string to allocate
  const int nValueLength{GetLexerLanguage(nullptr)};

  //Call the function which does the work
  CStringA sLanguage;
  GetLexerLanguage(sLanguage.GetBufferSetLength(nValueLength));
  sLanguage.ReleaseBuffer();

  return sLanguage;
}

CStringA CScintillaCtrl::PropertyNames()
{
  //Work out the length of string to allocate
  const int nValueLength{PropertyNames(nullptr)};

  //Call the function which does the work
  CStringA sPropertyNames;
  PropertyNames(sPropertyNames.GetBufferSetLength(nValueLength));
  sPropertyNames.ReleaseBuffer();

  return sPropertyNames;
}

CStringA CScintillaCtrl::EOLAnnotationGetText(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nValueLength{EOLAnnotationGetText(line, nullptr)};

  //Call the function which does the work
  CStringA sText;
  EOLAnnotationGetText(line, sText.GetBufferSetLength(nValueLength));
  sText.ReleaseBuffer();

  return sText;
}

CStringA CScintillaCtrl::DescribeProperty(_In_z_ const char* name)
{
  //Work out the length of string to allocate
  const int nValueLength{DescribeProperty(name, nullptr)};

  //Call the function which does the work
  CStringA sDescribeProperty;
  DescribeProperty(name, sDescribeProperty.GetBufferSetLength(nValueLength));
  sDescribeProperty.ReleaseBuffer();

  return sDescribeProperty;
}

CStringA CScintillaCtrl::DescribeKeyWordSets()
{
  //Work out the length of string to allocate
  const int nValueLength{DescribeKeyWordSets(nullptr)};

  //Call the function which does the work
  CStringA sDescribeKeyWordSets;
  DescribeKeyWordSets(sDescribeKeyWordSets.GetBufferSetLength(nValueLength));
  sDescribeKeyWordSets.ReleaseBuffer();

  return sDescribeKeyWordSets;
}

CStringA CScintillaCtrl::GetTag(_In_ int tagNumber)
{
  //Work out the length of string to allocate
  const int nValueLength{GetTag(tagNumber, nullptr)};

  //Call the function which does the work
  CStringA sTag;
  GetTag(tagNumber, sTag.GetBufferSetLength(nValueLength));
  sTag.ReleaseBuffer();

  return sTag;
}

CStringA CScintillaCtrl::GetWordChars()
{
  //Work out the length of string to allocate
  const int nValueLength{GetWordChars(nullptr)};

  //Call the function which does the work
  CStringA sWordChars;
  GetWordChars(sWordChars.GetBufferSetLength(nValueLength));
  sWordChars.ReleaseBuffer();

  return sWordChars;
}

CStringA CScintillaCtrl::GetWhitespaceChars()
{
  //Work out the length of string to allocate
  const int nValueLength{GetWhitespaceChars(nullptr)};

  //Call the function which does the work
  CStringA sWhitespaceChars;
  GetWhitespaceChars(sWhitespaceChars.GetBufferSetLength(nValueLength));
  sWhitespaceChars.ReleaseBuffer();

  return sWhitespaceChars;
}

CStringA CScintillaCtrl::GetPunctuationChars()
{
  //Work out the length of string to allocate
  const int nValueLength{GetPunctuationChars(nullptr)};

  //Call the function which does the work
  CStringA sPunctuationChars;
  GetPunctuationChars(sPunctuationChars.GetBufferSetLength(nValueLength));
  sPunctuationChars.ReleaseBuffer();

  return sPunctuationChars;
}

CStringA CScintillaCtrl::GetTargetText()
{
  //Work out the length of string to allocate
  const Sci_Position nValueLength{GetTargetText(nullptr)};

  //Call the function which does the work
  CStringA sTargetText;
#pragma warning(suppress: 26472)
  GetTargetText(sTargetText.GetBufferSetLength(static_cast<int>(nValueLength)));
  sTargetText.ReleaseBuffer();

  return sTargetText;
}

CStringA CScintillaCtrl::NameOfStyle(_In_ int style)
{
  //Work out the length of string to allocate
  const int nValueLength{NameOfStyle(style, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  NameOfStyle(style, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::TagsOfStyle(_In_ int style)
{
  //Work out the length of string to allocate
  const int nValueLength{TagsOfStyle(style, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  TagsOfStyle(style, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::DescriptionOfStyle(_In_ int style)
{
  //Work out the length of string to allocate
  const int nValueLength{DescriptionOfStyle(style, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  DescriptionOfStyle(style, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::GetDefaultFoldDisplayText()
{
  //Work out the length of string to allocate
  const int nValueLength{GetDefaultFoldDisplayText(nullptr)};

  //Call the function which does the work
  CStringA sValue;
  GetDefaultFoldDisplayText(sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::MarginGetText(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nValueLength{MarginGetText(line, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  GetDefaultFoldDisplayText(sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::GetFontLocale()
{
  //Work out the length of string to allocate
  const int nValueLength{GetFontLocale(nullptr)};

  //Call the function which does the work
  CStringA sValue;
  GetFontLocale(sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::MarginGetStyles(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nValueLength{MarginGetStyles(line, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  MarginGetStyles(line, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::AnnotationGetText(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nValueLength{AnnotationGetText(line, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  AnnotationGetText(line, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::AnnotationGetStyles(_In_ intptr_t line)
{
  //Work out the length of string to allocate
  const int nValueLength{AnnotationGetStyles(line, nullptr)};

  //Call the function which does the work
  CStringA sValue;
  AnnotationGetStyles(line, sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

CStringA CScintillaCtrl::GetSubStyleBases()
{
  //Work out the length of string to allocate
  const int nValueLength{GetSubStyleBases(nullptr)};

  //Call the function which does the work
  CStringA sValue;
  GetSubStyleBases(sValue.GetBufferSetLength(nValueLength));
  sValue.ReleaseBuffer();

  return sValue;
}

#endif //#ifdef _UNICODE


#pragma warning(push)
#pragma warning(disable : 26461 26472 26490)
void CScintillaCtrl::AddText(_In_ Sci_Position length, _In_reads_bytes_(length) const char* text)
{
  Call(SCI_ADDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
}

void CScintillaCtrl::AddStyledText(_In_ Sci_Position length, _In_reads_bytes_(length) char* c)
{
  Call(SCI_ADDSTYLEDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(c));
}

void CScintillaCtrl::InsertText(_In_ Sci_Position pos, _In_z_ const char* text)
{
  Call(SCI_INSERTTEXT, static_cast<WPARAM>(pos), reinterpret_cast<LPARAM>(text));
}

void CScintillaCtrl::ChangeInsertion(_In_ Sci_Position length, _In_reads_bytes_(length) const char* text)
{
  Call(SCI_CHANGEINSERTION, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
}

void CScintillaCtrl::ClearAll()
{
  Call(SCI_CLEARALL, 0, 0);
}

void CScintillaCtrl::DeleteRange(_In_ Sci_Position start, _In_ Sci_Position lengthDelete)
{
  Call(SCI_DELETERANGE, static_cast<WPARAM>(start), static_cast<LPARAM>(lengthDelete));
}

void CScintillaCtrl::ClearDocumentStyle()
{
  Call(SCI_CLEARDOCUMENTSTYLE, 0, 0);
}

Sci_Position CScintillaCtrl::GetLength()
{
  return static_cast<Sci_Position>(Call(SCI_GETLENGTH, 0, 0));
}

int CScintillaCtrl::GetCharAt(_In_ Sci_Position pos)
{
  return static_cast<int>(Call(SCI_GETCHARAT, static_cast<WPARAM>(pos), 0));
}

Sci_Position CScintillaCtrl::GetCurrentPos()
{
  return static_cast<Sci_Position>(Call(SCI_GETCURRENTPOS, 0, 0));
}

Sci_Position CScintillaCtrl::GetAnchor()
{
  return static_cast<Sci_Position>(Call(SCI_GETANCHOR, 0, 0));
}

int CScintillaCtrl::GetStyleAt(_In_ Sci_Position pos)
{
  return static_cast<int>(Call(SCI_GETSTYLEAT, static_cast<WPARAM>(pos), 0));
}

int CScintillaCtrl::GetStyleIndexAt(_In_ Sci_Position pos)
{
  return static_cast<int>(Call(SCI_GETSTYLEINDEXAT, static_cast<WPARAM>(pos), 0));
}

void CScintillaCtrl::Redo()
{
  Call(SCI_REDO, 0, 0);
}

void CScintillaCtrl::SetUndoCollection(_In_ BOOL collectUndo)
{
  Call(SCI_SETUNDOCOLLECTION, static_cast<WPARAM>(collectUndo), 0);
}

void CScintillaCtrl::SelectAll()
{
  Call(SCI_SELECTALL, 0, 0);
}

void CScintillaCtrl::SetSavePoint()
{
  Call(SCI_SETSAVEPOINT, 0, 0);
}

Sci_Position CScintillaCtrl::GetStyledText(_Inout_ Sci_TextRange* tr)
{
  return static_cast<Sci_Position>(Call(SCI_GETSTYLEDTEXT, 0, reinterpret_cast<LPARAM>(tr)));
}

BOOL CScintillaCtrl::CanRedo()
{
  return static_cast<BOOL>(Call(SCI_CANREDO, 0, 0));
}

intptr_t CScintillaCtrl::MarkerLineFromHandle(_In_ int markerHandle)
{
  return static_cast<intptr_t>(Call(SCI_MARKERLINEFROMHANDLE, static_cast<WPARAM>(markerHandle), 0));
}

void CScintillaCtrl::MarkerDeleteHandle(_In_ int markerHandle)
{
  Call(SCI_MARKERDELETEHANDLE, static_cast<WPARAM>(markerHandle), 0);
}

int CScintillaCtrl::MarkerHandleFromLine(_In_ intptr_t line, _In_ int which)
{
  return static_cast<int>(Call(SCI_MARKERHANDLEFROMLINE, static_cast<WPARAM>(line), static_cast<LPARAM>(which)));
}

int CScintillaCtrl::MarkerNumberFromLine(_In_ intptr_t line, _In_ int which)
{
  return static_cast<int>(Call(SCI_MARKERNUMBERFROMLINE, static_cast<WPARAM>(line), static_cast<LPARAM>(which)));
}

BOOL CScintillaCtrl::GetUndoCollection()
{
  return static_cast<BOOL>(Call(SCI_GETUNDOCOLLECTION, 0, 0));
}

int CScintillaCtrl::GetViewWS()
{
  return static_cast<int>(Call(SCI_GETVIEWWS, 0, 0));
}

void CScintillaCtrl::SetViewWS(_In_ int viewWS)
{
  Call(SCI_SETVIEWWS, static_cast<WPARAM>(viewWS), 0);
}

int CScintillaCtrl::GetTabDrawMode()
{
  return static_cast<int>(Call(SCI_GETTABDRAWMODE, 0, 0));
}

void CScintillaCtrl::SetTabDrawMode(_In_ int tabDrawMode)
{
  Call(SCI_SETTABDRAWMODE, static_cast<WPARAM>(tabDrawMode), 0);
}

Sci_Position CScintillaCtrl::PositionFromPoint(_In_ int x, _In_ int y)
{
  return static_cast<Sci_Position>(Call(SCI_POSITIONFROMPOINT, static_cast<WPARAM>(x), static_cast<LPARAM>(y)));
}

Sci_Position CScintillaCtrl::PositionFromPointClose(_In_ int x, _In_ int y)
{
  return static_cast<Sci_Position>(Call(SCI_POSITIONFROMPOINTCLOSE, static_cast<WPARAM>(x), static_cast<LPARAM>(y)));
}

void CScintillaCtrl::GotoLine(_In_ intptr_t line)
{
  Call(SCI_GOTOLINE, static_cast<WPARAM>(line), 0);
}

void CScintillaCtrl::GotoPos(_In_ Sci_Position caret)
{
  Call(SCI_GOTOPOS, static_cast<WPARAM>(caret), 0);
}

void CScintillaCtrl::SetAnchor(_In_ Sci_Position anchor)
{
  Call(SCI_SETANCHOR, static_cast<WPARAM>(anchor), 0);
}

Sci_Position CScintillaCtrl::GetCurLine(_In_ Sci_Position length, _Inout_opt_z_ char* text)
{
  return static_cast<Sci_Position>(Call(SCI_GETCURLINE, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text)));
}

Sci_Position CScintillaCtrl::GetEndStyled()
{
  return static_cast<Sci_Position>(Call(SCI_GETENDSTYLED, 0, 0));
}

void CScintillaCtrl::ConvertEOLs(_In_ int eolMode)
{
  Call(SCI_CONVERTEOLS, static_cast<WPARAM>(eolMode), 0);
}

int CScintillaCtrl::GetEOLMode()
{
  return static_cast<int>(Call(SCI_GETEOLMODE, 0, 0));
}

void CScintillaCtrl::SetEOLMode(_In_ int eolMode)
{
  Call(SCI_SETEOLMODE, static_cast<WPARAM>(eolMode), 0);
}

void CScintillaCtrl::StartStyling(_In_ Sci_Position start, _In_ int unused)
{
  Call(SCI_STARTSTYLING, static_cast<WPARAM>(start), static_cast<LPARAM>(unused));
}

void CScintillaCtrl::SetStyling(_In_ Sci_Position length, _In_ int style)
{
  Call(SCI_SETSTYLING, static_cast<WPARAM>(length), static_cast<LPARAM>(style));
}

BOOL CScintillaCtrl::GetBufferedDraw()
{
  return static_cast<BOOL>(Call(SCI_GETBUFFEREDDRAW, 0, 0));
}

void CScintillaCtrl::SetBufferedDraw(_In_ BOOL buffered)
{
  Call(SCI_SETBUFFEREDDRAW, static_cast<WPARAM>(buffered), 0);
}

void CScintillaCtrl::SetTabWidth(_In_ int tabWidth)
{
  Call(SCI_SETTABWIDTH, static_cast<WPARAM>(tabWidth), 0);
}

int CScintillaCtrl::GetTabWidth()
{
  return static_cast<int>(Call(SCI_GETTABWIDTH, 0, 0));
}

void CScintillaCtrl::SetTabMinimumWidth(_In_ int pixels)
{
  Call(SCI_SETTABMINIMUMWIDTH, static_cast<WPARAM>(pixels), 0);
}

int CScintillaCtrl::GetTabMinimumWidth()
{
  return static_cast<int>(Call(SCI_GETTABMINIMUMWIDTH, 0, 0));
}

void CScintillaCtrl::ClearTabStops(_In_ intptr_t line)
{
  Call(SCI_CLEARTABSTOPS, static_cast<WPARAM>(line), 0);
}

void CScintillaCtrl::AddTabStop(_In_ intptr_t line, _In_ int x)
{
  Call(SCI_ADDTABSTOP, static_cast<WPARAM>(line), static_cast<LPARAM>(x));
}

int CScintillaCtrl::GetNextTabStop(_In_ intptr_t line, _In_ int x)
{
  return static_cast<int>(Call(SCI_GETNEXTTABSTOP, static_cast<WPARAM>(line), static_cast<LPARAM>(x)));
}

void CScintillaCtrl::SetCodePage(_In_ int codePage)
{
  Call(SCI_SETCODEPAGE, static_cast<WPARAM>(codePage), 0);
}

void CScintillaCtrl::SetFontLocale(_In_z_ const char* localeName)
{
  Call(SCI_SETFONTLOCALE, 0, reinterpret_cast<LPARAM>(localeName));
}

int CScintillaCtrl::GetFontLocale(_Inout_opt_z_ char* localeName)
{
  return static_cast<int>(Call(SCI_GETFONTLOCALE, 0, reinterpret_cast<LPARAM>(localeName)));
}

int CScintillaCtrl::GetIMEInteraction()
{
  return static_cast<int>(Call(SCI_GETIMEINTERACTION, 0, 0));
}

void CScintillaCtrl::SetIMEInteraction(_In_ int imeInteraction)
{
  Call(SCI_SETIMEINTERACTION, static_cast<WPARAM>(imeInteraction), 0);
}

void CScintillaCtrl::MarkerDefine(_In_ int markerNumber, _In_ int markerSymbol)
{
  Call(SCI_MARKERDEFINE, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(markerSymbol));
}

void CScintillaCtrl::MarkerSetFore(_In_ int markerNumber, _In_ COLORREF fore)
{
  Call(SCI_MARKERSETFORE, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(fore));
}

void CScintillaCtrl::MarkerSetBack(_In_ int markerNumber, _In_ COLORREF back)
{
  Call(SCI_MARKERSETBACK, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(back));
}

void CScintillaCtrl::MarkerSetBackSelected(_In_ int markerNumber, _In_ COLORREF back)
{
  Call(SCI_MARKERSETBACKSELECTED, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(back));
}

void CScintillaCtrl::MarkerSetForeTranslucent(_In_ int markerNumber, _In_ int fore)
{
  Call(SCI_MARKERSETFORETRANSLUCENT, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(fore));
}

void CScintillaCtrl::MarkerSetBackTranslucent(_In_ int markerNumber, _In_ int back)
{
  Call(SCI_MARKERSETBACKTRANSLUCENT, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(back));
}

void CScintillaCtrl::MarkerSetBackSelectedTranslucent(_In_ int markerNumber, _In_ int back)
{
  Call(SCI_MARKERSETBACKSELECTEDTRANSLUCENT, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(back));
}

void CScintillaCtrl::MarkerSetStrokeWidth(_In_ int markerNumber, _In_ int hundredths)
{
  Call(SCI_MARKERSETSTROKEWIDTH, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(hundredths));
}

void CScintillaCtrl::MarkerEnableHighlight(_In_ BOOL enabled)
{
  Call(SCI_MARKERENABLEHIGHLIGHT, static_cast<WPARAM>(enabled), 0);
}

int CScintillaCtrl::MarkerAdd(_In_ intptr_t line, _In_ int markerNumber)
{
  return static_cast<int>(Call(SCI_MARKERADD, static_cast<WPARAM>(line), static_cast<LPARAM>(markerNumber)));
}

void CScintillaCtrl::MarkerDelete(_In_ intptr_t line, _In_ int markerNumber)
{
  Call(SCI_MARKERDELETE, static_cast<WPARAM>(line), static_cast<LPARAM>(markerNumber));
}

void CScintillaCtrl::MarkerDeleteAll(_In_ int markerNumber)
{
  Call(SCI_MARKERDELETEALL, static_cast<WPARAM>(markerNumber), 0);
}

int CScintillaCtrl::MarkerGet(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_MARKERGET, static_cast<WPARAM>(line), 0));
}

intptr_t CScintillaCtrl::MarkerNext(_In_ intptr_t lineStart, _In_ int markerMask)
{
  return static_cast<intptr_t>(Call(SCI_MARKERNEXT, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(markerMask)));
}

intptr_t CScintillaCtrl::MarkerPrevious(_In_ intptr_t lineStart, _In_ int markerMask)
{
  return static_cast<intptr_t>(Call(SCI_MARKERPREVIOUS, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(markerMask)));
}

void CScintillaCtrl::MarkerDefinePixmap(_In_ int markerNumber, _In_ const char* pixmap)
{
  Call(SCI_MARKERDEFINEPIXMAP, static_cast<WPARAM>(markerNumber), reinterpret_cast<LPARAM>(pixmap));
}

void CScintillaCtrl::MarkerAddSet(_In_ intptr_t line, _In_ int markerSet)
{
  Call(SCI_MARKERADDSET, static_cast<WPARAM>(line), static_cast<LPARAM>(markerSet));
}

void CScintillaCtrl::MarkerSetAlpha(_In_ int markerNumber, _In_ int alpha)
{
  Call(SCI_MARKERSETALPHA, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(alpha));
}

int CScintillaCtrl::MarkerGetLayer(_In_ int markerNumber)
{
  return static_cast<int>(Call(SCI_MARKERGETLAYER, static_cast<WPARAM>(markerNumber), 0));
}

void CScintillaCtrl::MarkerSetLayer(_In_ int markerNumber, _In_ int layer)
{
  Call(SCI_MARKERSETLAYER, static_cast<WPARAM>(markerNumber), static_cast<LPARAM>(layer));
}

void CScintillaCtrl::SetMarginTypeN(_In_ int margin, _In_ int marginType)
{
  Call(SCI_SETMARGINTYPEN, static_cast<WPARAM>(margin), static_cast<LPARAM>(marginType));
}

int CScintillaCtrl::GetMarginTypeN(_In_ int margin)
{
  return static_cast<int>(Call(SCI_GETMARGINTYPEN, static_cast<WPARAM>(margin), 0));
}

void CScintillaCtrl::SetMarginWidthN(_In_ int margin, _In_ int pixelWidth)
{
  Call(SCI_SETMARGINWIDTHN, static_cast<WPARAM>(margin), static_cast<LPARAM>(pixelWidth));
}

int CScintillaCtrl::GetMarginWidthN(_In_ int margin)
{
  return static_cast<int>(Call(SCI_GETMARGINWIDTHN, static_cast<WPARAM>(margin), 0));
}

void CScintillaCtrl::SetMarginMaskN(_In_ int margin, _In_ int mask)
{
  Call(SCI_SETMARGINMASKN, static_cast<WPARAM>(margin), static_cast<LPARAM>(mask));
}

int CScintillaCtrl::GetMarginMaskN(_In_ int margin)
{
  return static_cast<int>(Call(SCI_GETMARGINMASKN, static_cast<WPARAM>(margin), 0));
}

void CScintillaCtrl::SetMarginSensitiveN(_In_ int margin, _In_ BOOL sensitive)
{
  Call(SCI_SETMARGINSENSITIVEN, static_cast<WPARAM>(margin), static_cast<LPARAM>(sensitive));
}

BOOL CScintillaCtrl::GetMarginSensitiveN(_In_ int margin)
{
  return static_cast<BOOL>(Call(SCI_GETMARGINSENSITIVEN, static_cast<WPARAM>(margin), 0));
}

void CScintillaCtrl::SetMarginCursorN(_In_ int margin, _In_ int cursor)
{
  Call(SCI_SETMARGINCURSORN, static_cast<WPARAM>(margin), static_cast<LPARAM>(cursor));
}

int CScintillaCtrl::GetMarginCursorN(_In_ int margin)
{
  return static_cast<int>(Call(SCI_GETMARGINCURSORN, static_cast<WPARAM>(margin), 0));
}

void CScintillaCtrl::SetMarginBackN(_In_ int margin, _In_ COLORREF back)
{
  Call(SCI_SETMARGINBACKN, static_cast<WPARAM>(margin), static_cast<LPARAM>(back));
}

COLORREF CScintillaCtrl::GetMarginBackN(_In_ int margin)
{
  return static_cast<COLORREF>(Call(SCI_GETMARGINBACKN, static_cast<WPARAM>(margin), 0));
}

void CScintillaCtrl::SetMargins(_In_ int margins)
{
  Call(SCI_SETMARGINS, static_cast<WPARAM>(margins), 0);
}

int CScintillaCtrl::GetMargins()
{
  return static_cast<int>(Call(SCI_GETMARGINS, 0, 0));
}

void CScintillaCtrl::StyleClearAll()
{
  Call(SCI_STYLECLEARALL, 0, 0);
}

void CScintillaCtrl::StyleSetFore(_In_ int style, _In_ COLORREF fore)
{
  Call(SCI_STYLESETFORE, static_cast<WPARAM>(style), static_cast<LPARAM>(fore));
}

void CScintillaCtrl::StyleSetBack(_In_ int style, _In_ COLORREF back)
{
  Call(SCI_STYLESETBACK, static_cast<WPARAM>(style), static_cast<LPARAM>(back));
}

void CScintillaCtrl::StyleSetBold(_In_ int style, _In_ BOOL bold)
{
  Call(SCI_STYLESETBOLD, static_cast<WPARAM>(style), static_cast<LPARAM>(bold));
}

void CScintillaCtrl::StyleSetItalic(_In_ int style, _In_ BOOL italic)
{
  Call(SCI_STYLESETITALIC, static_cast<WPARAM>(style), static_cast<LPARAM>(italic));
}

void CScintillaCtrl::StyleSetSize(_In_ int style, _In_ int sizePoints)
{
  Call(SCI_STYLESETSIZE, static_cast<WPARAM>(style), static_cast<LPARAM>(sizePoints));
}

void CScintillaCtrl::StyleSetFont(_In_ int style, _In_z_ const char* fontName)
{
  Call(SCI_STYLESETFONT, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(fontName));
}

void CScintillaCtrl::StyleSetEOLFilled(_In_ int style, _In_ BOOL eolFilled)
{
  Call(SCI_STYLESETEOLFILLED, static_cast<WPARAM>(style), static_cast<LPARAM>(eolFilled));
}

void CScintillaCtrl::StyleResetDefault()
{
  Call(SCI_STYLERESETDEFAULT, 0, 0);
}

void CScintillaCtrl::StyleSetUnderline(_In_ int style, _In_ BOOL underline)
{
  Call(SCI_STYLESETUNDERLINE, static_cast<WPARAM>(style), static_cast<LPARAM>(underline));
}

COLORREF CScintillaCtrl::StyleGetFore(_In_ int style)
{
  return static_cast<COLORREF>(Call(SCI_STYLEGETFORE, static_cast<WPARAM>(style), 0));
}

COLORREF CScintillaCtrl::StyleGetBack(_In_ int style)
{
  return static_cast<COLORREF>(Call(SCI_STYLEGETBACK, static_cast<WPARAM>(style), 0));
}

BOOL CScintillaCtrl::StyleGetBold(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETBOLD, static_cast<WPARAM>(style), 0));
}

BOOL CScintillaCtrl::StyleGetItalic(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETITALIC, static_cast<WPARAM>(style), 0));
}

int CScintillaCtrl::StyleGetSize(_In_ int style)
{
  return static_cast<int>(Call(SCI_STYLEGETSIZE, static_cast<WPARAM>(style), 0));
}

int CScintillaCtrl::StyleGetFont(_In_ int style, _Inout_opt_z_ char* fontName)
{
  return static_cast<int>(Call(SCI_STYLEGETFONT, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(fontName)));
}

BOOL CScintillaCtrl::StyleGetEOLFilled(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETEOLFILLED, static_cast<WPARAM>(style), 0));
}

BOOL CScintillaCtrl::StyleGetUnderline(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETUNDERLINE, static_cast<WPARAM>(style), 0));
}

int CScintillaCtrl::StyleGetCase(_In_ int style)
{
  return static_cast<int>(Call(SCI_STYLEGETCASE, static_cast<WPARAM>(style), 0));
}

int CScintillaCtrl::StyleGetCharacterSet(_In_ int style)
{
  return static_cast<int>(Call(SCI_STYLEGETCHARACTERSET, static_cast<WPARAM>(style), 0));
}

BOOL CScintillaCtrl::StyleGetVisible(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETVISIBLE, static_cast<WPARAM>(style), 0));
}

BOOL CScintillaCtrl::StyleGetChangeable(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETCHANGEABLE, static_cast<WPARAM>(style), 0));
}

BOOL CScintillaCtrl::StyleGetHotSpot(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETHOTSPOT, static_cast<WPARAM>(style), 0));
}

void CScintillaCtrl::StyleSetCase(_In_ int style, _In_ int caseVisible)
{
  Call(SCI_STYLESETCASE, static_cast<WPARAM>(style), static_cast<LPARAM>(caseVisible));
}

void CScintillaCtrl::StyleSetSizeFractional(_In_ int style, _In_ int sizeInHundredthPoints)
{
  Call(SCI_STYLESETSIZEFRACTIONAL, static_cast<WPARAM>(style), static_cast<LPARAM>(sizeInHundredthPoints));
}

int CScintillaCtrl::StyleGetSizeFractional(_In_ int style)
{
  return static_cast<int>(Call(SCI_STYLEGETSIZEFRACTIONAL, static_cast<WPARAM>(style), 0));
}

void CScintillaCtrl::StyleSetWeight(_In_ int style, _In_ int weight)
{
  Call(SCI_STYLESETWEIGHT, static_cast<WPARAM>(style), static_cast<LPARAM>(weight));
}

int CScintillaCtrl::StyleGetWeight(_In_ int style)
{
  return static_cast<int>(Call(SCI_STYLEGETWEIGHT, static_cast<WPARAM>(style), 0));
}

void CScintillaCtrl::StyleSetCharacterSet(_In_ int style, _In_ int characterSet)
{
  Call(SCI_STYLESETCHARACTERSET, static_cast<WPARAM>(style), static_cast<LPARAM>(characterSet));
}

void CScintillaCtrl::StyleSetHotSpot(_In_ int style, _In_ BOOL hotspot)
{
  Call(SCI_STYLESETHOTSPOT, static_cast<WPARAM>(style), static_cast<LPARAM>(hotspot));
}

void CScintillaCtrl::StyleSetCheckMonospaced(_In_ int style, _In_ BOOL checkMonospaced)
{
  Call(SCI_STYLESETCHECKMONOSPACED, static_cast<WPARAM>(style), static_cast<LPARAM>(checkMonospaced));
}

BOOL CScintillaCtrl::StyleGetCheckMonospaced(_In_ int style)
{
  return static_cast<BOOL>(Call(SCI_STYLEGETCHECKMONOSPACED, static_cast<WPARAM>(style), 0));
}

void CScintillaCtrl::SetElementColour(_In_ int element, _In_ int colourElement)
{
  Call(SCI_SETELEMENTCOLOUR, static_cast<WPARAM>(element), static_cast<LPARAM>(colourElement));
}

int CScintillaCtrl::GetElementColour(_In_ int element)
{
  return static_cast<int>(Call(SCI_GETELEMENTCOLOUR, static_cast<WPARAM>(element), 0));
}

void CScintillaCtrl::ResetElementColour(_In_ int element)
{
  Call(SCI_RESETELEMENTCOLOUR, static_cast<WPARAM>(element), 0);
}

BOOL CScintillaCtrl::GetElementIsSet(_In_ int element)
{
  return static_cast<BOOL>(Call(SCI_GETELEMENTISSET, static_cast<WPARAM>(element), 0));
}

BOOL CScintillaCtrl::GetElementAllowsTranslucent(_In_ int element)
{
  return static_cast<BOOL>(Call(SCI_GETELEMENTALLOWSTRANSLUCENT, static_cast<WPARAM>(element), 0));
}

int CScintillaCtrl::GetElementBaseColour(_In_ int element)
{
  return static_cast<int>(Call(SCI_GETELEMENTBASECOLOUR, static_cast<WPARAM>(element), 0));
}

void CScintillaCtrl::SetSelFore(_In_ BOOL useSetting, _In_ COLORREF fore)
{
  Call(SCI_SETSELFORE, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore));
}

void CScintillaCtrl::SetSelBack(_In_ BOOL useSetting, _In_ COLORREF back)
{
  Call(SCI_SETSELBACK, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back));
}

int CScintillaCtrl::GetSelAlpha()
{
  return static_cast<int>(Call(SCI_GETSELALPHA, 0, 0));
}

void CScintillaCtrl::SetSelAlpha(_In_ int alpha)
{
  Call(SCI_SETSELALPHA, static_cast<WPARAM>(alpha), 0);
}

BOOL CScintillaCtrl::GetSelEOLFilled()
{
  return static_cast<BOOL>(Call(SCI_GETSELEOLFILLED, 0, 0));
}

void CScintillaCtrl::SetSelEOLFilled(_In_ BOOL filled)
{
  Call(SCI_SETSELEOLFILLED, static_cast<WPARAM>(filled), 0);
}

int CScintillaCtrl::GetSelectionLayer()
{
  return static_cast<int>(Call(SCI_GETSELECTIONLAYER, 0, 0));
}

void CScintillaCtrl::SetSelectionLayer(_In_ int layer)
{
  Call(SCI_SETSELECTIONLAYER, static_cast<WPARAM>(layer), 0);
}

int CScintillaCtrl::GetCaretLineLayer()
{
  return static_cast<int>(Call(SCI_GETCARETLINELAYER, 0, 0));
}

void CScintillaCtrl::SetCaretLineLayer(_In_ int layer)
{
  Call(SCI_SETCARETLINELAYER, static_cast<WPARAM>(layer), 0);
}

BOOL CScintillaCtrl::GetCaretLineHighlightSubLine()
{
  return static_cast<BOOL>(Call(SCI_GETCARETLINEHIGHLIGHTSUBLINE, 0, 0));
}

void CScintillaCtrl::SetCaretLineHighlightSubLine(_In_ BOOL subLine)
{
  Call(SCI_SETCARETLINEHIGHLIGHTSUBLINE, static_cast<WPARAM>(subLine), 0);
}

void CScintillaCtrl::SetCaretFore(_In_ COLORREF fore)
{
  Call(SCI_SETCARETFORE, static_cast<WPARAM>(fore), 0);
}

void CScintillaCtrl::AssignCmdKey(_In_ DWORD keyDefinition, _In_ int sciCommand)
{
  Call(SCI_ASSIGNCMDKEY, static_cast<WPARAM>(keyDefinition), static_cast<LPARAM>(sciCommand));
}

void CScintillaCtrl::ClearCmdKey(_In_ DWORD keyDefinition)
{
  Call(SCI_CLEARCMDKEY, static_cast<WPARAM>(keyDefinition), 0);
}

void CScintillaCtrl::ClearAllCmdKeys()
{
  Call(SCI_CLEARALLCMDKEYS, 0, 0);
}

void CScintillaCtrl::SetStylingEx(_In_ Sci_Position length, _In_ const char* styles)
{
  Call(SCI_SETSTYLINGEX, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(styles));
}

void CScintillaCtrl::StyleSetVisible(_In_ int style, _In_ BOOL visible)
{
  Call(SCI_STYLESETVISIBLE, static_cast<WPARAM>(style), static_cast<LPARAM>(visible));
}

int CScintillaCtrl::GetCaretPeriod()
{
  return static_cast<int>(Call(SCI_GETCARETPERIOD, 0, 0));
}

void CScintillaCtrl::SetCaretPeriod(_In_ int periodMilliseconds)
{
  Call(SCI_SETCARETPERIOD, static_cast<WPARAM>(periodMilliseconds), 0);
}

void CScintillaCtrl::SetWordChars(_In_ const char* characters)
{
  Call(SCI_SETWORDCHARS, 0, reinterpret_cast<LPARAM>(characters));
}

int CScintillaCtrl::GetWordChars(_Inout_opt_z_ char* characters)
{
  return static_cast<int>(Call(SCI_GETWORDCHARS, 0, reinterpret_cast<LPARAM>(characters)));
}

void CScintillaCtrl::SetCharacterCategoryOptimization(_In_ int countCharacters)
{
  Call(SCI_SETCHARACTERCATEGORYOPTIMIZATION, static_cast<WPARAM>(countCharacters), 0);
}

int CScintillaCtrl::GetCharacterCategoryOptimization()
{
  return static_cast<int>(Call(SCI_GETCHARACTERCATEGORYOPTIMIZATION, 0, 0));
}

void CScintillaCtrl::BeginUndoAction()
{
  Call(SCI_BEGINUNDOACTION, 0, 0);
}

void CScintillaCtrl::EndUndoAction()
{
  Call(SCI_ENDUNDOACTION, 0, 0);
}

void CScintillaCtrl::IndicSetStyle(_In_ int indicator, _In_ int indicatorStyle)
{
  Call(SCI_INDICSETSTYLE, static_cast<WPARAM>(indicator), static_cast<LPARAM>(indicatorStyle));
}

int CScintillaCtrl::IndicGetStyle(_In_ int indicator)
{
  return static_cast<int>(Call(SCI_INDICGETSTYLE, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::IndicSetFore(_In_ int indicator, _In_ COLORREF fore)
{
  Call(SCI_INDICSETFORE, static_cast<WPARAM>(indicator), static_cast<LPARAM>(fore));
}

COLORREF CScintillaCtrl::IndicGetFore(_In_ int indicator)
{
  return static_cast<COLORREF>(Call(SCI_INDICGETFORE, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::IndicSetUnder(_In_ int indicator, _In_ BOOL under)
{
  Call(SCI_INDICSETUNDER, static_cast<WPARAM>(indicator), static_cast<LPARAM>(under));
}

BOOL CScintillaCtrl::IndicGetUnder(_In_ int indicator)
{
  return static_cast<BOOL>(Call(SCI_INDICGETUNDER, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::IndicSetHoverStyle(_In_ int indicator, _In_ int indicatorStyle)
{
  Call(SCI_INDICSETHOVERSTYLE, static_cast<WPARAM>(indicator), static_cast<LPARAM>(indicatorStyle));
}

int CScintillaCtrl::IndicGetHoverStyle(_In_ int indicator)
{
  return static_cast<int>(Call(SCI_INDICGETHOVERSTYLE, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::IndicSetHoverFore(_In_ int indicator, _In_ COLORREF fore)
{
  Call(SCI_INDICSETHOVERFORE, static_cast<WPARAM>(indicator), static_cast<LPARAM>(fore));
}

COLORREF CScintillaCtrl::IndicGetHoverFore(_In_ int indicator)
{
  return static_cast<COLORREF>(Call(SCI_INDICGETHOVERFORE, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::IndicSetFlags(_In_ int indicator, _In_ int flags)
{
  Call(SCI_INDICSETFLAGS, static_cast<WPARAM>(indicator), static_cast<LPARAM>(flags));
}

int CScintillaCtrl::IndicGetFlags(_In_ int indicator)
{
  return static_cast<int>(Call(SCI_INDICGETFLAGS, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::IndicSetStrokeWidth(_In_ int indicator, _In_ int hundredths)
{
  Call(SCI_INDICSETSTROKEWIDTH, static_cast<WPARAM>(indicator), static_cast<LPARAM>(hundredths));
}

int CScintillaCtrl::IndicGetStrokeWidth(_In_ int indicator)
{
  return static_cast<int>(Call(SCI_INDICGETSTROKEWIDTH, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::SetWhitespaceFore(_In_ BOOL useSetting, _In_ COLORREF fore)
{
  Call(SCI_SETWHITESPACEFORE, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore));
}

void CScintillaCtrl::SetWhitespaceBack(_In_ BOOL useSetting, _In_ COLORREF back)
{
  Call(SCI_SETWHITESPACEBACK, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back));
}

void CScintillaCtrl::SetWhitespaceSize(_In_ int size)
{
  Call(SCI_SETWHITESPACESIZE, static_cast<WPARAM>(size), 0);
}

int CScintillaCtrl::GetWhitespaceSize()
{
  return static_cast<int>(Call(SCI_GETWHITESPACESIZE, 0, 0));
}

void CScintillaCtrl::SetLineState(_In_ intptr_t line, _In_ int state)
{
  Call(SCI_SETLINESTATE, static_cast<WPARAM>(line), static_cast<LPARAM>(state));
}

int CScintillaCtrl::GetLineState(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_GETLINESTATE, static_cast<WPARAM>(line), 0));
}

int CScintillaCtrl::GetMaxLineState()
{
  return static_cast<int>(Call(SCI_GETMAXLINESTATE, 0, 0));
}

BOOL CScintillaCtrl::GetCaretLineVisible()
{
  return static_cast<BOOL>(Call(SCI_GETCARETLINEVISIBLE, 0, 0));
}

void CScintillaCtrl::SetCaretLineVisible(_In_ BOOL show)
{
  Call(SCI_SETCARETLINEVISIBLE, static_cast<WPARAM>(show), 0);
}

COLORREF CScintillaCtrl::GetCaretLineBack()
{
  return static_cast<COLORREF>(Call(SCI_GETCARETLINEBACK, 0, 0));
}

void CScintillaCtrl::SetCaretLineBack(_In_ COLORREF back)
{
  Call(SCI_SETCARETLINEBACK, static_cast<WPARAM>(back), 0);
}

int CScintillaCtrl::GetCaretLineFrame()
{
  return static_cast<int>(Call(SCI_GETCARETLINEFRAME, 0, 0));
}

void CScintillaCtrl::SetCaretLineFrame(_In_ int width)
{
  Call(SCI_SETCARETLINEFRAME, static_cast<WPARAM>(width), 0);
}

void CScintillaCtrl::StyleSetChangeable(_In_ int style, _In_ BOOL changeable)
{
  Call(SCI_STYLESETCHANGEABLE, static_cast<WPARAM>(style), static_cast<LPARAM>(changeable));
}

void CScintillaCtrl::AutoCShow(_In_ Sci_Position lengthEntered, _In_z_ const char* itemList)
{
  Call(SCI_AUTOCSHOW, static_cast<WPARAM>(lengthEntered), reinterpret_cast<LPARAM>(itemList));
}

void CScintillaCtrl::AutoCCancel()
{
  Call(SCI_AUTOCCANCEL, 0, 0);
}

BOOL CScintillaCtrl::AutoCActive()
{
  return static_cast<BOOL>(Call(SCI_AUTOCACTIVE, 0, 0));
}

Sci_Position CScintillaCtrl::AutoCPosStart()
{
  return static_cast<Sci_Position>(Call(SCI_AUTOCPOSSTART, 0, 0));
}

void CScintillaCtrl::AutoCComplete()
{
  Call(SCI_AUTOCCOMPLETE, 0, 0);
}

void CScintillaCtrl::AutoCStops(_In_z_ const char* characterSet)
{
  Call(SCI_AUTOCSTOPS, 0, reinterpret_cast<LPARAM>(characterSet));
}

void CScintillaCtrl::AutoCSetSeparator(_In_ int separatorCharacter)
{
  Call(SCI_AUTOCSETSEPARATOR, static_cast<WPARAM>(separatorCharacter), 0);
}

int CScintillaCtrl::AutoCGetSeparator()
{
  return static_cast<int>(Call(SCI_AUTOCGETSEPARATOR, 0, 0));
}

void CScintillaCtrl::AutoCSelect(_In_z_ const char* select)
{
  Call(SCI_AUTOCSELECT, 0, reinterpret_cast<LPARAM>(select));
}

void CScintillaCtrl::AutoCSetCancelAtStart(_In_ BOOL cancel)
{
  Call(SCI_AUTOCSETCANCELATSTART, static_cast<WPARAM>(cancel), 0);
}

BOOL CScintillaCtrl::AutoCGetCancelAtStart()
{
  return static_cast<BOOL>(Call(SCI_AUTOCGETCANCELATSTART, 0, 0));
}

void CScintillaCtrl::AutoCSetFillUps(_In_z_ const char* characterSet)
{
  Call(SCI_AUTOCSETFILLUPS, 0, reinterpret_cast<LPARAM>(characterSet));
}

void CScintillaCtrl::AutoCSetChooseSingle(_In_ BOOL chooseSingle)
{
  Call(SCI_AUTOCSETCHOOSESINGLE, static_cast<WPARAM>(chooseSingle), 0);
}

BOOL CScintillaCtrl::AutoCGetChooseSingle()
{
  return static_cast<BOOL>(Call(SCI_AUTOCGETCHOOSESINGLE, 0, 0));
}

void CScintillaCtrl::AutoCSetIgnoreCase(_In_ BOOL ignoreCase)
{
  Call(SCI_AUTOCSETIGNORECASE, static_cast<WPARAM>(ignoreCase), 0);
}

BOOL CScintillaCtrl::AutoCGetIgnoreCase()
{
  return static_cast<BOOL>(Call(SCI_AUTOCGETIGNORECASE, 0, 0));
}

void CScintillaCtrl::UserListShow(_In_ int listType, _In_z_ const char* itemList)
{
  Call(SCI_USERLISTSHOW, static_cast<WPARAM>(listType), reinterpret_cast<LPARAM>(itemList));
}

void CScintillaCtrl::AutoCSetAutoHide(_In_ BOOL autoHide)
{
  Call(SCI_AUTOCSETAUTOHIDE, static_cast<WPARAM>(autoHide), 0);
}

BOOL CScintillaCtrl::AutoCGetAutoHide()
{
  return static_cast<BOOL>(Call(SCI_AUTOCGETAUTOHIDE, 0, 0));
}

void CScintillaCtrl::AutoCSetOptions(_In_ int options)
{
  Call(SCI_AUTOCSETOPTIONS, static_cast<WPARAM>(options), 0);
}

int CScintillaCtrl::AutoCGetOptions()
{
  return static_cast<int>(Call(SCI_AUTOCGETOPTIONS, 0, 0));
}

void CScintillaCtrl::AutoCSetDropRestOfWord(_In_ BOOL dropRestOfWord)
{
  Call(SCI_AUTOCSETDROPRESTOFWORD, static_cast<WPARAM>(dropRestOfWord), 0);
}

BOOL CScintillaCtrl::AutoCGetDropRestOfWord()
{
  return static_cast<BOOL>(Call(SCI_AUTOCGETDROPRESTOFWORD, 0, 0));
}

void CScintillaCtrl::RegisterImage(_In_ int type, _In_ const char* xpmData)
{
  Call(SCI_REGISTERIMAGE, static_cast<WPARAM>(type), reinterpret_cast<LPARAM>(xpmData));
}

void CScintillaCtrl::ClearRegisteredImages()
{
  Call(SCI_CLEARREGISTEREDIMAGES, 0, 0);
}

int CScintillaCtrl::AutoCGetTypeSeparator()
{
  return static_cast<int>(Call(SCI_AUTOCGETTYPESEPARATOR, 0, 0));
}

void CScintillaCtrl::AutoCSetTypeSeparator(_In_ int separatorCharacter)
{
  Call(SCI_AUTOCSETTYPESEPARATOR, static_cast<WPARAM>(separatorCharacter), 0);
}

void CScintillaCtrl::AutoCSetMaxWidth(_In_ int characterCount)
{
  Call(SCI_AUTOCSETMAXWIDTH, static_cast<WPARAM>(characterCount), 0);
}

int CScintillaCtrl::AutoCGetMaxWidth()
{
  return static_cast<int>(Call(SCI_AUTOCGETMAXWIDTH, 0, 0));
}

void CScintillaCtrl::AutoCSetMaxHeight(_In_ int rowCount)
{
  Call(SCI_AUTOCSETMAXHEIGHT, static_cast<WPARAM>(rowCount), 0);
}

int CScintillaCtrl::AutoCGetMaxHeight()
{
  return static_cast<int>(Call(SCI_AUTOCGETMAXHEIGHT, 0, 0));
}

void CScintillaCtrl::SetIndent(_In_ int indentSize)
{
  Call(SCI_SETINDENT, static_cast<WPARAM>(indentSize), 0);
}

int CScintillaCtrl::GetIndent()
{
  return static_cast<int>(Call(SCI_GETINDENT, 0, 0));
}

void CScintillaCtrl::SetUseTabs(_In_ BOOL useTabs)
{
  Call(SCI_SETUSETABS, static_cast<WPARAM>(useTabs), 0);
}

BOOL CScintillaCtrl::GetUseTabs()
{
  return static_cast<BOOL>(Call(SCI_GETUSETABS, 0, 0));
}

void CScintillaCtrl::SetLineIndentation(_In_ intptr_t line, _In_ int indentation)
{
  Call(SCI_SETLINEINDENTATION, static_cast<WPARAM>(line), static_cast<LPARAM>(indentation));
}

int CScintillaCtrl::GetLineIndentation(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_GETLINEINDENTATION, static_cast<WPARAM>(line), 0));
}

Sci_Position CScintillaCtrl::GetLineIndentPosition(_In_ intptr_t line)
{
  return static_cast<Sci_Position>(Call(SCI_GETLINEINDENTPOSITION, static_cast<WPARAM>(line), 0));
}

Sci_Position CScintillaCtrl::GetColumn(_In_ Sci_Position pos)
{
  return static_cast<Sci_Position>(Call(SCI_GETCOLUMN, static_cast<WPARAM>(pos), 0));
}

Sci_Position CScintillaCtrl::CountCharacters(_In_ Sci_Position start, _In_ Sci_Position end)
{
  return static_cast<Sci_Position>(Call(SCI_COUNTCHARACTERS, static_cast<WPARAM>(start), static_cast<LPARAM>(end)));
}

Sci_Position CScintillaCtrl::CountCodeUnits(_In_ Sci_Position start, _In_ Sci_Position end)
{
  return static_cast<Sci_Position>(Call(SCI_COUNTCODEUNITS, static_cast<WPARAM>(start), static_cast<LPARAM>(end)));
}

void CScintillaCtrl::SetHScrollBar(_In_ BOOL visible)
{
  Call(SCI_SETHSCROLLBAR, static_cast<WPARAM>(visible), 0);
}

BOOL CScintillaCtrl::GetHScrollBar()
{
  return static_cast<BOOL>(Call(SCI_GETHSCROLLBAR, 0, 0));
}

void CScintillaCtrl::SetIndentationGuides(_In_ int indentView)
{
  Call(SCI_SETINDENTATIONGUIDES, static_cast<WPARAM>(indentView), 0);
}

int CScintillaCtrl::GetIndentationGuides()
{
  return static_cast<int>(Call(SCI_GETINDENTATIONGUIDES, 0, 0));
}

void CScintillaCtrl::SetHighlightGuide(_In_ Sci_Position column)
{
  Call(SCI_SETHIGHLIGHTGUIDE, static_cast<WPARAM>(column), 0);
}

Sci_Position CScintillaCtrl::GetHighlightGuide()
{
  return static_cast<Sci_Position>(Call(SCI_GETHIGHLIGHTGUIDE, 0, 0));
}

Sci_Position CScintillaCtrl::GetLineEndPosition(_In_ intptr_t line)
{
  return static_cast<Sci_Position>(Call(SCI_GETLINEENDPOSITION, static_cast<WPARAM>(line), 0));
}

int CScintillaCtrl::GetCodePage()
{
  return static_cast<int>(Call(SCI_GETCODEPAGE, 0, 0));
}

COLORREF CScintillaCtrl::GetCaretFore()
{
  return static_cast<COLORREF>(Call(SCI_GETCARETFORE, 0, 0));
}

BOOL CScintillaCtrl::GetReadOnly()
{
  return static_cast<BOOL>(Call(SCI_GETREADONLY, 0, 0));
}

void CScintillaCtrl::SetCurrentPos(_In_ Sci_Position caret)
{
  Call(SCI_SETCURRENTPOS, static_cast<WPARAM>(caret), 0);
}

void CScintillaCtrl::SetSelectionStart(_In_ Sci_Position anchor)
{
  Call(SCI_SETSELECTIONSTART, static_cast<WPARAM>(anchor), 0);
}

Sci_Position CScintillaCtrl::GetSelectionStart()
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONSTART, 0, 0));
}

void CScintillaCtrl::SetSelectionEnd(_In_ Sci_Position caret)
{
  Call(SCI_SETSELECTIONEND, static_cast<WPARAM>(caret), 0);
}

Sci_Position CScintillaCtrl::GetSelectionEnd()
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONEND, 0, 0));
}

void CScintillaCtrl::SetEmptySelection(_In_ Sci_Position caret)
{
  Call(SCI_SETEMPTYSELECTION, static_cast<WPARAM>(caret), 0);
}

void CScintillaCtrl::SetPrintMagnification(_In_ int magnification)
{
  Call(SCI_SETPRINTMAGNIFICATION, static_cast<WPARAM>(magnification), 0);
}

int CScintillaCtrl::GetPrintMagnification()
{
  return static_cast<int>(Call(SCI_GETPRINTMAGNIFICATION, 0, 0));
}

void CScintillaCtrl::SetPrintColourMode(_In_ int mode)
{
  Call(SCI_SETPRINTCOLOURMODE, static_cast<WPARAM>(mode), 0);
}

int CScintillaCtrl::GetPrintColourMode()
{
  return static_cast<int>(Call(SCI_GETPRINTCOLOURMODE, 0, 0));
}

Sci_Position CScintillaCtrl::FindText(_In_ int searchFlags, _Inout_ Sci_TextToFind* ft)
{
  return static_cast<Sci_Position>(Call(SCI_FINDTEXT, static_cast<WPARAM>(searchFlags), reinterpret_cast<LPARAM>(ft)));
}

Sci_Position CScintillaCtrl::FindTextFull(_In_ int searchFlags, _Inout_ Sci_TextToFindFull* ft)
{
  return static_cast<Sci_Position>(Call(SCI_FINDTEXTFULL, static_cast<WPARAM>(searchFlags), reinterpret_cast<LPARAM>(ft)));
}

Sci_Position CScintillaCtrl::FormatRange(_In_ BOOL draw, _In_ Sci_RangeToFormat* fr)
{
  return static_cast<Sci_Position>(Call(SCI_FORMATRANGE, static_cast<WPARAM>(draw), reinterpret_cast<LPARAM>(fr)));
}

Sci_Position CScintillaCtrl::FormatRangeFull(_In_ BOOL draw, _In_ Sci_RangeToFormatFull* fr)
{
  return static_cast<Sci_Position>(Call(SCI_FORMATRANGEFULL, static_cast<WPARAM>(draw), reinterpret_cast<LPARAM>(fr)));
}

intptr_t CScintillaCtrl::GetFirstVisibleLine()
{
  return static_cast<intptr_t>(Call(SCI_GETFIRSTVISIBLELINE, 0, 0));
}

Sci_Position CScintillaCtrl::GetLine(_In_ intptr_t line, _Inout_opt_z_ char* text)
{
  return static_cast<Sci_Position>(Call(SCI_GETLINE, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text)));
}

intptr_t CScintillaCtrl::GetLineCount()
{
  return static_cast<intptr_t>(Call(SCI_GETLINECOUNT, 0, 0));
}

void CScintillaCtrl::AllocateLines(_In_ intptr_t lines)
{
  Call(SCI_ALLOCATELINES, static_cast<WPARAM>(lines), 0);
}

void CScintillaCtrl::SetMarginLeft(_In_ int pixelWidth)
{
  Call(SCI_SETMARGINLEFT, 0, static_cast<LPARAM>(pixelWidth));
}

int CScintillaCtrl::GetMarginLeft()
{
  return static_cast<int>(Call(SCI_GETMARGINLEFT, 0, 0));
}

void CScintillaCtrl::SetMarginRight(_In_ int pixelWidth)
{
  Call(SCI_SETMARGINRIGHT, 0, static_cast<LPARAM>(pixelWidth));
}

int CScintillaCtrl::GetMarginRight()
{
  return static_cast<int>(Call(SCI_GETMARGINRIGHT, 0, 0));
}

BOOL CScintillaCtrl::GetModify()
{
  return static_cast<BOOL>(Call(SCI_GETMODIFY, 0, 0));
}

void CScintillaCtrl::SetSel(_In_ Sci_Position anchor, _In_ Sci_Position caret)
{
  Call(SCI_SETSEL, static_cast<WPARAM>(anchor), static_cast<LPARAM>(caret));
}

Sci_Position CScintillaCtrl::GetSelText(_Inout_opt_z_ char* text)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELTEXT, 0, reinterpret_cast<LPARAM>(text)));
}

Sci_Position CScintillaCtrl::GetTextRange(_Inout_ Sci_TextRange* tr)
{
  return static_cast<Sci_Position>(Call(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(tr)));
}

Sci_Position CScintillaCtrl::GetTextRangeFull(_Inout_ Sci_TextRangeFull* tr)
{
  return static_cast<Sci_Position>(Call(SCI_GETTEXTRANGEFULL, 0, reinterpret_cast<LPARAM>(tr)));
}

void CScintillaCtrl::HideSelection(_In_ BOOL hide)
{
  Call(SCI_HIDESELECTION, static_cast<WPARAM>(hide), 0);
}

int CScintillaCtrl::PointXFromPosition(_In_ Sci_Position pos)
{
  return static_cast<int>(Call(SCI_POINTXFROMPOSITION, 0, static_cast<LPARAM>(pos)));
}

int CScintillaCtrl::PointYFromPosition(_In_ Sci_Position pos)
{
  return static_cast<int>(Call(SCI_POINTYFROMPOSITION, 0, static_cast<LPARAM>(pos)));
}

intptr_t CScintillaCtrl::LineFromPosition(_In_ Sci_Position pos)
{
  return static_cast<intptr_t>(Call(SCI_LINEFROMPOSITION, static_cast<WPARAM>(pos), 0));
}

Sci_Position CScintillaCtrl::PositionFromLine(_In_ intptr_t line)
{
  return static_cast<Sci_Position>(Call(SCI_POSITIONFROMLINE, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::LineScroll(_In_ Sci_Position columns, _In_ intptr_t lines)
{
  Call(SCI_LINESCROLL, static_cast<WPARAM>(columns), static_cast<LPARAM>(lines));
}

void CScintillaCtrl::ScrollCaret()
{
  Call(SCI_SCROLLCARET, 0, 0);
}

void CScintillaCtrl::ScrollRange(_In_ Sci_Position secondary, _In_ Sci_Position primary)
{
  Call(SCI_SCROLLRANGE, static_cast<WPARAM>(secondary), static_cast<LPARAM>(primary));
}

void CScintillaCtrl::ReplaceSel(_In_z_ const char* text)
{
  Call(SCI_REPLACESEL, 0, reinterpret_cast<LPARAM>(text));
}

void CScintillaCtrl::SetReadOnly(_In_ BOOL readOnly)
{
  Call(SCI_SETREADONLY, static_cast<WPARAM>(readOnly), 0);
}

void CScintillaCtrl::Null()
{
  Call(SCI_NULL, 0, 0);
}

BOOL CScintillaCtrl::CanPaste()
{
  return static_cast<BOOL>(Call(SCI_CANPASTE, 0, 0));
}

BOOL CScintillaCtrl::CanUndo()
{
  return static_cast<BOOL>(Call(SCI_CANUNDO, 0, 0));
}

void CScintillaCtrl::EmptyUndoBuffer()
{
  Call(SCI_EMPTYUNDOBUFFER, 0, 0);
}

void CScintillaCtrl::Undo()
{
  Call(SCI_UNDO, 0, 0);
}

void CScintillaCtrl::Cut()
{
  Call(SCI_CUT, 0, 0);
}

void CScintillaCtrl::Copy()
{
  Call(SCI_COPY, 0, 0);
}

void CScintillaCtrl::Paste()
{
  Call(SCI_PASTE, 0, 0);
}

void CScintillaCtrl::Clear()
{
  Call(SCI_CLEAR, 0, 0);
}

void CScintillaCtrl::SetText(_In_z_ const char* text)
{
  Call(SCI_SETTEXT, 0, reinterpret_cast<LPARAM>(text));
}

Sci_Position CScintillaCtrl::GetText(_In_ Sci_Position length, _Inout_updates_opt_(length+1) char* text)
{
  return static_cast<Sci_Position>(Call(SCI_GETTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text)));
}

Sci_Position CScintillaCtrl::GetTextLength()
{
  return static_cast<Sci_Position>(Call(SCI_GETTEXTLENGTH, 0, 0));
}

SciFnDirectStatus CScintillaCtrl::GetDirectStatusFunction()
{
  return reinterpret_cast<SciFnDirectStatus>(Call(SCI_GETDIRECTSTATUSFUNCTION, 0, 0));
}

void CScintillaCtrl::SetOvertype(_In_ BOOL overType)
{
  Call(SCI_SETOVERTYPE, static_cast<WPARAM>(overType), 0);
}

BOOL CScintillaCtrl::GetOvertype()
{
  return static_cast<BOOL>(Call(SCI_GETOVERTYPE, 0, 0));
}

void CScintillaCtrl::SetCaretWidth(_In_ int pixelWidth)
{
  Call(SCI_SETCARETWIDTH, static_cast<WPARAM>(pixelWidth), 0);
}

int CScintillaCtrl::GetCaretWidth()
{
  return static_cast<int>(Call(SCI_GETCARETWIDTH, 0, 0));
}

void CScintillaCtrl::SetTargetStart(_In_ Sci_Position start)
{
  Call(SCI_SETTARGETSTART, static_cast<WPARAM>(start), 0);
}

Sci_Position CScintillaCtrl::GetTargetStart()
{
  return static_cast<Sci_Position>(Call(SCI_GETTARGETSTART, 0, 0));
}

void CScintillaCtrl::SetTargetStartVirtualSpace(_In_ Sci_Position space)
{
  Call(SCI_SETTARGETSTARTVIRTUALSPACE, static_cast<WPARAM>(space), 0);
}

Sci_Position CScintillaCtrl::GetTargetStartVirtualSpace()
{
  return static_cast<Sci_Position>(Call(SCI_GETTARGETSTARTVIRTUALSPACE, 0, 0));
}

void CScintillaCtrl::SetTargetEnd(_In_ Sci_Position end)
{
  Call(SCI_SETTARGETEND, static_cast<WPARAM>(end), 0);
}

void CScintillaCtrl::SetTargetRange(_In_ Sci_Position start, _In_ Sci_Position end)
{
  Call(SCI_SETTARGETRANGE, static_cast<WPARAM>(start), static_cast<LPARAM>(end));
}

Sci_Position CScintillaCtrl::GetTargetText(_Inout_opt_z_ char* text)
{
  return static_cast<Sci_Position>(Call(SCI_GETTARGETTEXT, 0, reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::TargetFromSelection()
{
  Call(SCI_TARGETFROMSELECTION, 0, 0);
}

void CScintillaCtrl::TargetWholeDocument()
{
  Call(SCI_TARGETWHOLEDOCUMENT, 0, 0);
}

Sci_Position CScintillaCtrl::GetTargetEnd()
{
  return static_cast<Sci_Position>(Call(SCI_GETTARGETEND, 0, 0));
}

void CScintillaCtrl::SetTargetEndVirtualSpace(_In_ Sci_Position space)
{
  Call(SCI_SETTARGETENDVIRTUALSPACE, static_cast<WPARAM>(space), 0);
}

Sci_Position CScintillaCtrl::GetTargetEndVirtualSpace()
{
  return static_cast<Sci_Position>(Call(SCI_GETTARGETENDVIRTUALSPACE, 0, 0));
}

Sci_Position CScintillaCtrl::ReplaceTarget(_In_ Sci_Position length, _In_ const char* text)
{
  return static_cast<Sci_Position>(Call(SCI_REPLACETARGET, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text)));
}

Sci_Position CScintillaCtrl::ReplaceTargetRE(_In_ Sci_Position length, _In_ const char* text)
{
  return static_cast<Sci_Position>(Call(SCI_REPLACETARGETRE, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text)));
}

Sci_Position CScintillaCtrl::SearchInTarget(_In_ Sci_Position length, _In_reads_bytes_(length) const char* text)
{
  return static_cast<Sci_Position>(Call(SCI_SEARCHINTARGET, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::SetSearchFlags(_In_ int searchFlags)
{
  Call(SCI_SETSEARCHFLAGS, static_cast<WPARAM>(searchFlags), 0);
}

int CScintillaCtrl::GetSearchFlags()
{
  return static_cast<int>(Call(SCI_GETSEARCHFLAGS, 0, 0));
}

void CScintillaCtrl::CallTipShow(_In_ Sci_Position pos, _In_z_ const char* definition)
{
  Call(SCI_CALLTIPSHOW, static_cast<WPARAM>(pos), reinterpret_cast<LPARAM>(definition));
}

void CScintillaCtrl::CallTipCancel()
{
  Call(SCI_CALLTIPCANCEL, 0, 0);
}

BOOL CScintillaCtrl::CallTipActive()
{
  return static_cast<BOOL>(Call(SCI_CALLTIPACTIVE, 0, 0));
}

Sci_Position CScintillaCtrl::CallTipPosStart()
{
  return static_cast<Sci_Position>(Call(SCI_CALLTIPPOSSTART, 0, 0));
}

void CScintillaCtrl::CallTipSetPosStart(_In_ Sci_Position posStart)
{
  Call(SCI_CALLTIPSETPOSSTART, static_cast<WPARAM>(posStart), 0);
}

void CScintillaCtrl::CallTipSetHlt(_In_ Sci_Position highlightStart, _In_ Sci_Position highlightEnd)
{
  Call(SCI_CALLTIPSETHLT, static_cast<WPARAM>(highlightStart), static_cast<LPARAM>(highlightEnd));
}

void CScintillaCtrl::CallTipSetBack(_In_ COLORREF back)
{
  Call(SCI_CALLTIPSETBACK, static_cast<WPARAM>(back), 0);
}

void CScintillaCtrl::CallTipSetFore(_In_ COLORREF fore)
{
  Call(SCI_CALLTIPSETFORE, static_cast<WPARAM>(fore), 0);
}

void CScintillaCtrl::CallTipSetForeHlt(_In_ COLORREF fore)
{
  Call(SCI_CALLTIPSETFOREHLT, static_cast<WPARAM>(fore), 0);
}

void CScintillaCtrl::CallTipUseStyle(_In_ int tabSize)
{
  Call(SCI_CALLTIPUSESTYLE, static_cast<WPARAM>(tabSize), 0);
}

void CScintillaCtrl::CallTipSetPosition(_In_ BOOL above)
{
  Call(SCI_CALLTIPSETPOSITION, static_cast<WPARAM>(above), 0);
}

intptr_t CScintillaCtrl::VisibleFromDocLine(_In_ intptr_t docLine)
{
  return static_cast<intptr_t>(Call(SCI_VISIBLEFROMDOCLINE, static_cast<WPARAM>(docLine), 0));
}

intptr_t CScintillaCtrl::DocLineFromVisible(_In_ intptr_t displayLine)
{
  return static_cast<intptr_t>(Call(SCI_DOCLINEFROMVISIBLE, static_cast<WPARAM>(displayLine), 0));
}

intptr_t CScintillaCtrl::WrapCount(_In_ intptr_t docLine)
{
  return static_cast<intptr_t>(Call(SCI_WRAPCOUNT, static_cast<WPARAM>(docLine), 0));
}

void CScintillaCtrl::SetFoldLevel(_In_ intptr_t line, _In_ int level)
{
  Call(SCI_SETFOLDLEVEL, static_cast<WPARAM>(line), static_cast<LPARAM>(level));
}

int CScintillaCtrl::GetFoldLevel(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_GETFOLDLEVEL, static_cast<WPARAM>(line), 0));
}

intptr_t CScintillaCtrl::GetLastChild(_In_ intptr_t line, _In_ int level)
{
  return static_cast<intptr_t>(Call(SCI_GETLASTCHILD, static_cast<WPARAM>(line), static_cast<LPARAM>(level)));
}

intptr_t CScintillaCtrl::GetFoldParent(_In_ intptr_t line)
{
  return static_cast<intptr_t>(Call(SCI_GETFOLDPARENT, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::ShowLines(_In_ intptr_t lineStart, _In_ intptr_t lineEnd)
{
  Call(SCI_SHOWLINES, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(lineEnd));
}

void CScintillaCtrl::HideLines(_In_ intptr_t lineStart, _In_ intptr_t lineEnd)
{
  Call(SCI_HIDELINES, static_cast<WPARAM>(lineStart), static_cast<LPARAM>(lineEnd));
}

BOOL CScintillaCtrl::GetLineVisible(_In_ intptr_t line)
{
  return static_cast<BOOL>(Call(SCI_GETLINEVISIBLE, static_cast<WPARAM>(line), 0));
}

BOOL CScintillaCtrl::GetAllLinesVisible()
{
  return static_cast<BOOL>(Call(SCI_GETALLLINESVISIBLE, 0, 0));
}

void CScintillaCtrl::SetFoldExpanded(_In_ intptr_t line, _In_ BOOL expanded)
{
  Call(SCI_SETFOLDEXPANDED, static_cast<WPARAM>(line), static_cast<LPARAM>(expanded));
}

BOOL CScintillaCtrl::GetFoldExpanded(_In_ intptr_t line)
{
  return static_cast<BOOL>(Call(SCI_GETFOLDEXPANDED, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::ToggleFold(_In_ intptr_t line)
{
  Call(SCI_TOGGLEFOLD, static_cast<WPARAM>(line), 0);
}

void CScintillaCtrl::ToggleFoldShowText(_In_ intptr_t line, _In_ const char* text)
{
  Call(SCI_TOGGLEFOLDSHOWTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
}

void CScintillaCtrl::FoldDisplayTextSetStyle(_In_ int style)
{
  Call(SCI_FOLDDISPLAYTEXTSETSTYLE, static_cast<WPARAM>(style), 0);
}

int CScintillaCtrl::FoldDisplayTextGetStyle()
{
  return static_cast<int>(Call(SCI_FOLDDISPLAYTEXTGETSTYLE, 0, 0));
}

void CScintillaCtrl::SetDefaultFoldDisplayText(_In_z_ const char* text)
{
  Call(SCI_SETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<LPARAM>(text));
}

int CScintillaCtrl::GetDefaultFoldDisplayText(_Inout_opt_z_ char* text)
{
  return static_cast<int>(Call(SCI_GETDEFAULTFOLDDISPLAYTEXT, 0, reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::FoldLine(_In_ intptr_t line, _In_ int action)
{
  Call(SCI_FOLDLINE, static_cast<WPARAM>(line), static_cast<LPARAM>(action));
}

void CScintillaCtrl::FoldChildren(_In_ intptr_t line, _In_ int action)
{
  Call(SCI_FOLDCHILDREN, static_cast<WPARAM>(line), static_cast<LPARAM>(action));
}

void CScintillaCtrl::ExpandChildren(_In_ intptr_t line, _In_ int level)
{
  Call(SCI_EXPANDCHILDREN, static_cast<WPARAM>(line), static_cast<LPARAM>(level));
}

void CScintillaCtrl::FoldAll(_In_ int action)
{
  Call(SCI_FOLDALL, static_cast<WPARAM>(action), 0);
}

void CScintillaCtrl::EnsureVisible(_In_ intptr_t line)
{
  Call(SCI_ENSUREVISIBLE, static_cast<WPARAM>(line), 0);
}

void CScintillaCtrl::SetAutomaticFold(_In_ int automaticFold)
{
  Call(SCI_SETAUTOMATICFOLD, static_cast<WPARAM>(automaticFold), 0);
}

int CScintillaCtrl::GetAutomaticFold()
{
  return static_cast<int>(Call(SCI_GETAUTOMATICFOLD, 0, 0));
}

void CScintillaCtrl::SetFoldFlags(_In_ int flags)
{
  Call(SCI_SETFOLDFLAGS, static_cast<WPARAM>(flags), 0);
}

void CScintillaCtrl::EnsureVisibleEnforcePolicy(_In_ intptr_t line)
{
  Call(SCI_ENSUREVISIBLEENFORCEPOLICY, static_cast<WPARAM>(line), 0);
}

void CScintillaCtrl::SetTabIndents(_In_ BOOL tabIndents)
{
  Call(SCI_SETTABINDENTS, static_cast<WPARAM>(tabIndents), 0);
}

BOOL CScintillaCtrl::GetTabIndents()
{
  return static_cast<BOOL>(Call(SCI_GETTABINDENTS, 0, 0));
}

void CScintillaCtrl::SetBackSpaceUnIndents(_In_ BOOL bsUnIndents)
{
  Call(SCI_SETBACKSPACEUNINDENTS, static_cast<WPARAM>(bsUnIndents), 0);
}

BOOL CScintillaCtrl::GetBackSpaceUnIndents()
{
  return static_cast<BOOL>(Call(SCI_GETBACKSPACEUNINDENTS, 0, 0));
}

void CScintillaCtrl::SetMouseDwellTime(_In_ int periodMilliseconds)
{
  Call(SCI_SETMOUSEDWELLTIME, static_cast<WPARAM>(periodMilliseconds), 0);
}

int CScintillaCtrl::GetMouseDwellTime()
{
  return static_cast<int>(Call(SCI_GETMOUSEDWELLTIME, 0, 0));
}

Sci_Position CScintillaCtrl::WordStartPosition(_In_ Sci_Position pos, _In_ BOOL onlyWordCharacters)
{
  return static_cast<Sci_Position>(Call(SCI_WORDSTARTPOSITION, static_cast<WPARAM>(pos), static_cast<LPARAM>(onlyWordCharacters)));
}

Sci_Position CScintillaCtrl::WordEndPosition(_In_ Sci_Position pos, _In_ BOOL onlyWordCharacters)
{
  return static_cast<Sci_Position>(Call(SCI_WORDENDPOSITION, static_cast<WPARAM>(pos), static_cast<LPARAM>(onlyWordCharacters)));
}

BOOL CScintillaCtrl::IsRangeWord(_In_ Sci_Position start, _In_ Sci_Position end)
{
  return static_cast<BOOL>(Call(SCI_ISRANGEWORD, static_cast<WPARAM>(start), static_cast<LPARAM>(end)));
}

void CScintillaCtrl::SetIdleStyling(_In_ int idleStyling)
{
  Call(SCI_SETIDLESTYLING, static_cast<WPARAM>(idleStyling), 0);
}

int CScintillaCtrl::GetIdleStyling()
{
  return static_cast<int>(Call(SCI_GETIDLESTYLING, 0, 0));
}

void CScintillaCtrl::SetWrapMode(_In_ int wrapMode)
{
  Call(SCI_SETWRAPMODE, static_cast<WPARAM>(wrapMode), 0);
}

int CScintillaCtrl::GetWrapMode()
{
  return static_cast<int>(Call(SCI_GETWRAPMODE, 0, 0));
}

void CScintillaCtrl::SetWrapVisualFlags(_In_ int wrapVisualFlags)
{
  Call(SCI_SETWRAPVISUALFLAGS, static_cast<WPARAM>(wrapVisualFlags), 0);
}

int CScintillaCtrl::GetWrapVisualFlags()
{
  return static_cast<int>(Call(SCI_GETWRAPVISUALFLAGS, 0, 0));
}

void CScintillaCtrl::SetWrapVisualFlagsLocation(_In_ int wrapVisualFlagsLocation)
{
  Call(SCI_SETWRAPVISUALFLAGSLOCATION, static_cast<WPARAM>(wrapVisualFlagsLocation), 0);
}

int CScintillaCtrl::GetWrapVisualFlagsLocation()
{
  return static_cast<int>(Call(SCI_GETWRAPVISUALFLAGSLOCATION, 0, 0));
}

void CScintillaCtrl::SetWrapStartIndent(_In_ int indent)
{
  Call(SCI_SETWRAPSTARTINDENT, static_cast<WPARAM>(indent), 0);
}

int CScintillaCtrl::GetWrapStartIndent()
{
  return static_cast<int>(Call(SCI_GETWRAPSTARTINDENT, 0, 0));
}

void CScintillaCtrl::SetWrapIndentMode(_In_ int wrapIndentMode)
{
  Call(SCI_SETWRAPINDENTMODE, static_cast<WPARAM>(wrapIndentMode), 0);
}

int CScintillaCtrl::GetWrapIndentMode()
{
  return static_cast<int>(Call(SCI_GETWRAPINDENTMODE, 0, 0));
}

void CScintillaCtrl::SetLayoutCache(_In_ int cacheMode)
{
  Call(SCI_SETLAYOUTCACHE, static_cast<WPARAM>(cacheMode), 0);
}

int CScintillaCtrl::GetLayoutCache()
{
  return static_cast<int>(Call(SCI_GETLAYOUTCACHE, 0, 0));
}

void CScintillaCtrl::SetScrollWidth(_In_ int pixelWidth)
{
  Call(SCI_SETSCROLLWIDTH, static_cast<WPARAM>(pixelWidth), 0);
}

int CScintillaCtrl::GetScrollWidth()
{
  return static_cast<int>(Call(SCI_GETSCROLLWIDTH, 0, 0));
}

void CScintillaCtrl::SetScrollWidthTracking(_In_ BOOL tracking)
{
  Call(SCI_SETSCROLLWIDTHTRACKING, static_cast<WPARAM>(tracking), 0);
}

BOOL CScintillaCtrl::GetScrollWidthTracking()
{
  return static_cast<BOOL>(Call(SCI_GETSCROLLWIDTHTRACKING, 0, 0));
}

int CScintillaCtrl::TextWidth(_In_ int style, _In_z_ const char* text)
{
  return static_cast<int>(Call(SCI_TEXTWIDTH, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::SetEndAtLastLine(_In_ BOOL endAtLastLine)
{
  Call(SCI_SETENDATLASTLINE, static_cast<WPARAM>(endAtLastLine), 0);
}

BOOL CScintillaCtrl::GetEndAtLastLine()
{
  return static_cast<BOOL>(Call(SCI_GETENDATLASTLINE, 0, 0));
}

int CScintillaCtrl::TextHeight(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_TEXTHEIGHT, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::SetVScrollBar(_In_ BOOL visible)
{
  Call(SCI_SETVSCROLLBAR, static_cast<WPARAM>(visible), 0);
}

BOOL CScintillaCtrl::GetVScrollBar()
{
  return static_cast<BOOL>(Call(SCI_GETVSCROLLBAR, 0, 0));
}

void CScintillaCtrl::AppendText(_In_ Sci_Position length, _In_reads_bytes_(length) const char* text)
{
  Call(SCI_APPENDTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
}

int CScintillaCtrl::GetPhasesDraw()
{
  return static_cast<int>(Call(SCI_GETPHASESDRAW, 0, 0));
}

void CScintillaCtrl::SetPhasesDraw(_In_ int phases)
{
  Call(SCI_SETPHASESDRAW, static_cast<WPARAM>(phases), 0);
}

void CScintillaCtrl::SetFontQuality(_In_ int fontQuality)
{
  Call(SCI_SETFONTQUALITY, static_cast<WPARAM>(fontQuality), 0);
}

int CScintillaCtrl::GetFontQuality()
{
  return static_cast<int>(Call(SCI_GETFONTQUALITY, 0, 0));
}

void CScintillaCtrl::SetFirstVisibleLine(_In_ intptr_t displayLine)
{
  Call(SCI_SETFIRSTVISIBLELINE, static_cast<WPARAM>(displayLine), 0);
}

void CScintillaCtrl::SetMultiPaste(_In_ int multiPaste)
{
  Call(SCI_SETMULTIPASTE, static_cast<WPARAM>(multiPaste), 0);
}

int CScintillaCtrl::GetMultiPaste()
{
  return static_cast<int>(Call(SCI_GETMULTIPASTE, 0, 0));
}

int CScintillaCtrl::GetTag(_In_ int tagNumber, _Inout_opt_z_ char* tagValue)
{
  return static_cast<int>(Call(SCI_GETTAG, static_cast<WPARAM>(tagNumber), reinterpret_cast<LPARAM>(tagValue)));
}

void CScintillaCtrl::LinesJoin()
{
  Call(SCI_LINESJOIN, 0, 0);
}

void CScintillaCtrl::LinesSplit(_In_ int pixelWidth)
{
  Call(SCI_LINESSPLIT, static_cast<WPARAM>(pixelWidth), 0);
}

void CScintillaCtrl::SetFoldMarginColour(_In_ BOOL useSetting, _In_ COLORREF back)
{
  Call(SCI_SETFOLDMARGINCOLOUR, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back));
}

void CScintillaCtrl::SetFoldMarginHiColour(_In_ BOOL useSetting, _In_ COLORREF fore)
{
  Call(SCI_SETFOLDMARGINHICOLOUR, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore));
}

void CScintillaCtrl::SetAccessibility(_In_ int accessibility)
{
  Call(SCI_SETACCESSIBILITY, static_cast<WPARAM>(accessibility), 0);
}

int CScintillaCtrl::GetAccessibility()
{
  return static_cast<int>(Call(SCI_GETACCESSIBILITY, 0, 0));
}

void CScintillaCtrl::LineDown()
{
  Call(SCI_LINEDOWN, 0, 0);
}

void CScintillaCtrl::LineDownExtend()
{
  Call(SCI_LINEDOWNEXTEND, 0, 0);
}

void CScintillaCtrl::LineUp()
{
  Call(SCI_LINEUP, 0, 0);
}

void CScintillaCtrl::LineUpExtend()
{
  Call(SCI_LINEUPEXTEND, 0, 0);
}

void CScintillaCtrl::CharLeft()
{
  Call(SCI_CHARLEFT, 0, 0);
}

void CScintillaCtrl::CharLeftExtend()
{
  Call(SCI_CHARLEFTEXTEND, 0, 0);
}

void CScintillaCtrl::CharRight()
{
  Call(SCI_CHARRIGHT, 0, 0);
}

void CScintillaCtrl::CharRightExtend()
{
  Call(SCI_CHARRIGHTEXTEND, 0, 0);
}

void CScintillaCtrl::WordLeft()
{
  Call(SCI_WORDLEFT, 0, 0);
}

void CScintillaCtrl::WordLeftExtend()
{
  Call(SCI_WORDLEFTEXTEND, 0, 0);
}

void CScintillaCtrl::WordRight()
{
  Call(SCI_WORDRIGHT, 0, 0);
}

void CScintillaCtrl::WordRightExtend()
{
  Call(SCI_WORDRIGHTEXTEND, 0, 0);
}

void CScintillaCtrl::Home()
{
  Call(SCI_HOME, 0, 0);
}

void CScintillaCtrl::HomeExtend()
{
  Call(SCI_HOMEEXTEND, 0, 0);
}

void CScintillaCtrl::LineEnd()
{
  Call(SCI_LINEEND, 0, 0);
}

void CScintillaCtrl::LineEndExtend()
{
  Call(SCI_LINEENDEXTEND, 0, 0);
}

void CScintillaCtrl::DocumentStart()
{
  Call(SCI_DOCUMENTSTART, 0, 0);
}

void CScintillaCtrl::DocumentStartExtend()
{
  Call(SCI_DOCUMENTSTARTEXTEND, 0, 0);
}

void CScintillaCtrl::DocumentEnd()
{
  Call(SCI_DOCUMENTEND, 0, 0);
}

void CScintillaCtrl::DocumentEndExtend()
{
  Call(SCI_DOCUMENTENDEXTEND, 0, 0);
}

void CScintillaCtrl::PageUp()
{
  Call(SCI_PAGEUP, 0, 0);
}

void CScintillaCtrl::PageUpExtend()
{
  Call(SCI_PAGEUPEXTEND, 0, 0);
}

void CScintillaCtrl::PageDown()
{
  Call(SCI_PAGEDOWN, 0, 0);
}

void CScintillaCtrl::PageDownExtend()
{
  Call(SCI_PAGEDOWNEXTEND, 0, 0);
}

void CScintillaCtrl::EditToggleOvertype()
{
  Call(SCI_EDITTOGGLEOVERTYPE, 0, 0);
}

void CScintillaCtrl::Cancel()
{
  Call(SCI_CANCEL, 0, 0);
}

void CScintillaCtrl::DeleteBack()
{
  Call(SCI_DELETEBACK, 0, 0);
}

void CScintillaCtrl::Tab()
{
  Call(SCI_TAB, 0, 0);
}

void CScintillaCtrl::BackTab()
{
  Call(SCI_BACKTAB, 0, 0);
}

void CScintillaCtrl::NewLine()
{
  Call(SCI_NEWLINE, 0, 0);
}

void CScintillaCtrl::FormFeed()
{
  Call(SCI_FORMFEED, 0, 0);
}

void CScintillaCtrl::VCHome()
{
  Call(SCI_VCHOME, 0, 0);
}

void CScintillaCtrl::VCHomeExtend()
{
  Call(SCI_VCHOMEEXTEND, 0, 0);
}

void CScintillaCtrl::ZoomIn()
{
  Call(SCI_ZOOMIN, 0, 0);
}

void CScintillaCtrl::ZoomOut()
{
  Call(SCI_ZOOMOUT, 0, 0);
}

void CScintillaCtrl::DelWordLeft()
{
  Call(SCI_DELWORDLEFT, 0, 0);
}

void CScintillaCtrl::DelWordRight()
{
  Call(SCI_DELWORDRIGHT, 0, 0);
}

void CScintillaCtrl::DelWordRightEnd()
{
  Call(SCI_DELWORDRIGHTEND, 0, 0);
}

void CScintillaCtrl::LineCut()
{
  Call(SCI_LINECUT, 0, 0);
}

void CScintillaCtrl::LineDelete()
{
  Call(SCI_LINEDELETE, 0, 0);
}

void CScintillaCtrl::LineTranspose()
{
  Call(SCI_LINETRANSPOSE, 0, 0);
}

void CScintillaCtrl::LineReverse()
{
  Call(SCI_LINEREVERSE, 0, 0);
}

void CScintillaCtrl::LineDuplicate()
{
  Call(SCI_LINEDUPLICATE, 0, 0);
}

void CScintillaCtrl::LowerCase()
{
  Call(SCI_LOWERCASE, 0, 0);
}

void CScintillaCtrl::UpperCase()
{
  Call(SCI_UPPERCASE, 0, 0);
}

void CScintillaCtrl::LineScrollDown()
{
  Call(SCI_LINESCROLLDOWN, 0, 0);
}

void CScintillaCtrl::LineScrollUp()
{
  Call(SCI_LINESCROLLUP, 0, 0);
}

void CScintillaCtrl::DeleteBackNotLine()
{
  Call(SCI_DELETEBACKNOTLINE, 0, 0);
}

void CScintillaCtrl::HomeDisplay()
{
  Call(SCI_HOMEDISPLAY, 0, 0);
}

void CScintillaCtrl::HomeDisplayExtend()
{
  Call(SCI_HOMEDISPLAYEXTEND, 0, 0);
}

void CScintillaCtrl::LineEndDisplay()
{
  Call(SCI_LINEENDDISPLAY, 0, 0);
}

void CScintillaCtrl::LineEndDisplayExtend()
{
  Call(SCI_LINEENDDISPLAYEXTEND, 0, 0);
}

void CScintillaCtrl::HomeWrap()
{
  Call(SCI_HOMEWRAP, 0, 0);
}

void CScintillaCtrl::HomeWrapExtend()
{
  Call(SCI_HOMEWRAPEXTEND, 0, 0);
}

void CScintillaCtrl::LineEndWrap()
{
  Call(SCI_LINEENDWRAP, 0, 0);
}

void CScintillaCtrl::LineEndWrapExtend()
{
  Call(SCI_LINEENDWRAPEXTEND, 0, 0);
}

void CScintillaCtrl::VCHomeWrap()
{
  Call(SCI_VCHOMEWRAP, 0, 0);
}

void CScintillaCtrl::VCHomeWrapExtend()
{
  Call(SCI_VCHOMEWRAPEXTEND, 0, 0);
}

void CScintillaCtrl::LineCopy()
{
  Call(SCI_LINECOPY, 0, 0);
}

void CScintillaCtrl::MoveCaretInsideView()
{
  Call(SCI_MOVECARETINSIDEVIEW, 0, 0);
}

Sci_Position CScintillaCtrl::LineLength(_In_ intptr_t line)
{
  return static_cast<Sci_Position>(Call(SCI_LINELENGTH, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::BraceHighlight(_In_ Sci_Position posA, _In_ Sci_Position posB)
{
  Call(SCI_BRACEHIGHLIGHT, static_cast<WPARAM>(posA), static_cast<LPARAM>(posB));
}

void CScintillaCtrl::BraceHighlightIndicator(_In_ BOOL useSetting, _In_ int indicator)
{
  Call(SCI_BRACEHIGHLIGHTINDICATOR, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(indicator));
}

void CScintillaCtrl::BraceBadLight(_In_ Sci_Position pos)
{
  Call(SCI_BRACEBADLIGHT, static_cast<WPARAM>(pos), 0);
}

void CScintillaCtrl::BraceBadLightIndicator(_In_ BOOL useSetting, _In_ int indicator)
{
  Call(SCI_BRACEBADLIGHTINDICATOR, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(indicator));
}

Sci_Position CScintillaCtrl::BraceMatch(_In_ Sci_Position pos, _In_ int maxReStyle)
{
  return static_cast<Sci_Position>(Call(SCI_BRACEMATCH, static_cast<WPARAM>(pos), maxReStyle));
}

Sci_Position CScintillaCtrl::BraceMatchNext(_In_ Sci_Position pos, _In_ Sci_Position startPos)
{
  return static_cast<Sci_Position>(Call(SCI_BRACEMATCHNEXT, static_cast<WPARAM>(pos), static_cast<LPARAM>(startPos)));
}

BOOL CScintillaCtrl::GetViewEOL()
{
  return static_cast<BOOL>(Call(SCI_GETVIEWEOL, 0, 0));
}

void CScintillaCtrl::SetViewEOL(_In_ BOOL visible)
{
  Call(SCI_SETVIEWEOL, static_cast<WPARAM>(visible), 0);
}

void* CScintillaCtrl::GetDocPointer()
{
#pragma warning(suppress: 26487)
  return reinterpret_cast<void*>(Call(SCI_GETDOCPOINTER, 0, 0));
}

void CScintillaCtrl::SetDocPointer(_In_opt_ void* doc)
{
  Call(SCI_SETDOCPOINTER, 0, reinterpret_cast<LPARAM>(doc));
}

void CScintillaCtrl::SetModEventMask(_In_ int eventMask)
{
  Call(SCI_SETMODEVENTMASK, static_cast<WPARAM>(eventMask), 0);
}

Sci_Position CScintillaCtrl::GetEdgeColumn()
{
  return static_cast<Sci_Position>(Call(SCI_GETEDGECOLUMN, 0, 0));
}

void CScintillaCtrl::SetEdgeColumn(_In_ Sci_Position column)
{
  Call(SCI_SETEDGECOLUMN, static_cast<WPARAM>(column), 0);
}

int CScintillaCtrl::GetEdgeMode()
{
  return static_cast<int>(Call(SCI_GETEDGEMODE, 0, 0));
}

void CScintillaCtrl::SetEdgeMode(_In_ int edgeMode)
{
  Call(SCI_SETEDGEMODE, static_cast<WPARAM>(edgeMode), 0);
}

COLORREF CScintillaCtrl::GetEdgeColour()
{
  return static_cast<COLORREF>(Call(SCI_GETEDGECOLOUR, 0, 0));
}

void CScintillaCtrl::SetEdgeColour(_In_ COLORREF edgeColour)
{
  Call(SCI_SETEDGECOLOUR, static_cast<WPARAM>(edgeColour), 0);
}

void CScintillaCtrl::MultiEdgeAddLine(_In_ Sci_Position column, _In_ COLORREF edgeColour)
{
  Call(SCI_MULTIEDGEADDLINE, static_cast<WPARAM>(column), static_cast<LPARAM>(edgeColour));
}

void CScintillaCtrl::MultiEdgeClearAll()
{
  Call(SCI_MULTIEDGECLEARALL, 0, 0);
}

Sci_Position CScintillaCtrl::GetMultiEdgeColumn(_In_ int which)
{
  return static_cast<Sci_Position>(Call(SCI_GETMULTIEDGECOLUMN, static_cast<WPARAM>(which), 0));
}

void CScintillaCtrl::SearchAnchor()
{
  Call(SCI_SEARCHANCHOR, 0, 0);
}

Sci_Position CScintillaCtrl::SearchNext(_In_ int searchFlags, _In_z_ const char* text)
{
  return static_cast<Sci_Position>(Call(SCI_SEARCHNEXT, static_cast<WPARAM>(searchFlags), reinterpret_cast<LPARAM>(text)));
}

Sci_Position CScintillaCtrl::SearchPrev(_In_ int searchFlags, _In_z_ const char* text)
{
  return static_cast<Sci_Position>(Call(SCI_SEARCHPREV, static_cast<WPARAM>(searchFlags), reinterpret_cast<LPARAM>(text)));
}

intptr_t CScintillaCtrl::LinesOnScreen()
{
  return static_cast<intptr_t>(Call(SCI_LINESONSCREEN, 0, 0));
}

void CScintillaCtrl::UsePopUp(_In_ int popUpMode)
{
  Call(SCI_USEPOPUP, static_cast<WPARAM>(popUpMode), 0);
}

BOOL CScintillaCtrl::SelectionIsRectangle()
{
  return static_cast<BOOL>(Call(SCI_SELECTIONISRECTANGLE, 0, 0));
}

void CScintillaCtrl::SetZoom(_In_ int zoomInPoints)
{
  Call(SCI_SETZOOM, static_cast<WPARAM>(zoomInPoints), 0);
}

int CScintillaCtrl::GetZoom()
{
  return static_cast<int>(Call(SCI_GETZOOM, 0, 0));
}

void* CScintillaCtrl::CreateDocument(_In_ Sci_Position bytes, _In_ int documentOptions)
{
#pragma warning(suppress: 26487)
  return reinterpret_cast<void*>(Call(SCI_CREATEDOCUMENT, static_cast<WPARAM>(bytes), static_cast<LPARAM>(documentOptions)));
}

void CScintillaCtrl::AddRefDocument(_In_ void* doc)
{
  Call(SCI_ADDREFDOCUMENT, 0, reinterpret_cast<LPARAM>(doc));
}

void CScintillaCtrl::ReleaseDocument(_In_ void* doc)
{
  Call(SCI_RELEASEDOCUMENT, 0, reinterpret_cast<LPARAM>(doc));
}

int CScintillaCtrl::GetDocumentOptions()
{
  return static_cast<int>(Call(SCI_GETDOCUMENTOPTIONS, 0, 0));
}

int CScintillaCtrl::GetModEventMask()
{
  return static_cast<int>(Call(SCI_GETMODEVENTMASK, 0, 0));
}

void CScintillaCtrl::SetCommandEvents(_In_ BOOL commandEvents)
{
  Call(SCI_SETCOMMANDEVENTS, static_cast<WPARAM>(commandEvents), 0);
}

BOOL CScintillaCtrl::GetCommandEvents()
{
  return static_cast<BOOL>(Call(SCI_GETCOMMANDEVENTS, 0, 0));
}

void CScintillaCtrl::SCISetFocus(_In_ BOOL focus)
{
  Call(SCI_SETFOCUS, static_cast<WPARAM>(focus), 0);
}

BOOL CScintillaCtrl::GetFocus()
{
  return static_cast<BOOL>(Call(SCI_GETFOCUS, 0, 0));
}

void CScintillaCtrl::SetStatus(_In_ int status)
{
  Call(SCI_SETSTATUS, static_cast<WPARAM>(status), 0);
}

int CScintillaCtrl::GetStatus()
{
  return static_cast<int>(Call(SCI_GETSTATUS, 0, 0));
}

void CScintillaCtrl::SetMouseDownCaptures(_In_ BOOL captures)
{
  Call(SCI_SETMOUSEDOWNCAPTURES, static_cast<WPARAM>(captures), 0);
}

BOOL CScintillaCtrl::GetMouseDownCaptures()
{
  return static_cast<BOOL>(Call(SCI_GETMOUSEDOWNCAPTURES, 0, 0));
}

void CScintillaCtrl::SetMouseWheelCaptures(_In_ BOOL captures)
{
  Call(SCI_SETMOUSEWHEELCAPTURES, static_cast<WPARAM>(captures), 0);
}

BOOL CScintillaCtrl::GetMouseWheelCaptures()
{
  return static_cast<BOOL>(Call(SCI_GETMOUSEWHEELCAPTURES, 0, 0));
}

void CScintillaCtrl::SetCursor(_In_ int cursorType)
{
  Call(SCI_SETCURSOR, static_cast<WPARAM>(cursorType), 0);
}

int CScintillaCtrl::GetCursor()
{
  return static_cast<int>(Call(SCI_GETCURSOR, 0, 0));
}

void CScintillaCtrl::SetControlCharSymbol(_In_ int symbol)
{
  Call(SCI_SETCONTROLCHARSYMBOL, static_cast<WPARAM>(symbol), 0);
}

int CScintillaCtrl::GetControlCharSymbol()
{
  return static_cast<int>(Call(SCI_GETCONTROLCHARSYMBOL, 0, 0));
}

void CScintillaCtrl::WordPartLeft()
{
  Call(SCI_WORDPARTLEFT, 0, 0);
}

void CScintillaCtrl::WordPartLeftExtend()
{
  Call(SCI_WORDPARTLEFTEXTEND, 0, 0);
}

void CScintillaCtrl::WordPartRight()
{
  Call(SCI_WORDPARTRIGHT, 0, 0);
}

void CScintillaCtrl::WordPartRightExtend()
{
  Call(SCI_WORDPARTRIGHTEXTEND, 0, 0);
}

void CScintillaCtrl::SetVisiblePolicy(_In_ int visiblePolicy, _In_ int visibleSlop)
{
  Call(SCI_SETVISIBLEPOLICY, static_cast<WPARAM>(visiblePolicy), static_cast<LPARAM>(visibleSlop));
}

void CScintillaCtrl::DelLineLeft()
{
  Call(SCI_DELLINELEFT, 0, 0);
}

void CScintillaCtrl::DelLineRight()
{
  Call(SCI_DELLINERIGHT, 0, 0);
}

void CScintillaCtrl::SetXOffset(_In_ int xOffset)
{
  Call(SCI_SETXOFFSET, static_cast<WPARAM>(xOffset), 0);
}

int CScintillaCtrl::GetXOffset()
{
  return static_cast<int>(Call(SCI_GETXOFFSET, 0, 0));
}

void CScintillaCtrl::ChooseCaretX()
{
  Call(SCI_CHOOSECARETX, 0, 0);
}

void CScintillaCtrl::GrabFocus()
{
  Call(SCI_GRABFOCUS, 0, 0);
}

void CScintillaCtrl::SetXCaretPolicy(_In_ int caretPolicy, _In_ int caretSlop)
{
  Call(SCI_SETXCARETPOLICY, static_cast<WPARAM>(caretPolicy), static_cast<LPARAM>(caretSlop));
}

void CScintillaCtrl::SetYCaretPolicy(_In_ int caretPolicy, _In_ int caretSlop)
{
  Call(SCI_SETYCARETPOLICY, static_cast<WPARAM>(caretPolicy), static_cast<LPARAM>(caretSlop));
}

void CScintillaCtrl::SetPrintWrapMode(_In_ int wrapMode)
{
  Call(SCI_SETPRINTWRAPMODE, static_cast<WPARAM>(wrapMode), 0);
}

int CScintillaCtrl::GetPrintWrapMode()
{
  return static_cast<int>(Call(SCI_GETPRINTWRAPMODE, 0, 0));
}

void CScintillaCtrl::SetHotspotActiveFore(_In_ BOOL useSetting, _In_ COLORREF fore)
{
  Call(SCI_SETHOTSPOTACTIVEFORE, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(fore));
}

COLORREF CScintillaCtrl::GetHotspotActiveFore()
{
  return static_cast<COLORREF>(Call(SCI_GETHOTSPOTACTIVEFORE, 0, 0));
}

void CScintillaCtrl::SetHotspotActiveBack(_In_ BOOL useSetting, _In_ COLORREF back)
{
  Call(SCI_SETHOTSPOTACTIVEBACK, static_cast<WPARAM>(useSetting), static_cast<LPARAM>(back));
}

COLORREF CScintillaCtrl::GetHotspotActiveBack()
{
  return static_cast<COLORREF>(Call(SCI_GETHOTSPOTACTIVEBACK, 0, 0));
}

void CScintillaCtrl::SetHotspotActiveUnderline(_In_ BOOL underline)
{
  Call(SCI_SETHOTSPOTACTIVEUNDERLINE, static_cast<WPARAM>(underline), 0);
}

BOOL CScintillaCtrl::GetHotspotActiveUnderline()
{
  return static_cast<BOOL>(Call(SCI_GETHOTSPOTACTIVEUNDERLINE, 0, 0));
}

void CScintillaCtrl::SetHotspotSingleLine(_In_ BOOL singleLine)
{
  Call(SCI_SETHOTSPOTSINGLELINE, static_cast<WPARAM>(singleLine), 0);
}

BOOL CScintillaCtrl::GetHotspotSingleLine()
{
  return static_cast<BOOL>(Call(SCI_GETHOTSPOTSINGLELINE, 0, 0));
}

void CScintillaCtrl::ParaDown()
{
  Call(SCI_PARADOWN, 0, 0);
}

void CScintillaCtrl::ParaDownExtend()
{
  Call(SCI_PARADOWNEXTEND, 0, 0);
}

void CScintillaCtrl::ParaUp()
{
  Call(SCI_PARAUP, 0, 0);
}

void CScintillaCtrl::ParaUpExtend()
{
  Call(SCI_PARAUPEXTEND, 0, 0);
}

Sci_Position CScintillaCtrl::PositionBefore(_In_ Sci_Position pos)
{
  return static_cast<Sci_Position>(Call(SCI_POSITIONBEFORE, static_cast<WPARAM>(pos), 0));
}

Sci_Position CScintillaCtrl::PositionAfter(_In_ Sci_Position pos)
{
  return static_cast<Sci_Position>(Call(SCI_POSITIONAFTER, static_cast<WPARAM>(pos), 0));
}

Sci_Position CScintillaCtrl::PositionRelative(_In_ Sci_Position pos, _In_ Sci_Position relative)
{
  return static_cast<Sci_Position>(Call(SCI_POSITIONRELATIVE, static_cast<WPARAM>(pos), static_cast<LPARAM>(relative)));
}

Sci_Position CScintillaCtrl::PositionRelativeCodeUnits(_In_ Sci_Position pos, _In_ Sci_Position relative)
{
  return static_cast<Sci_Position>(Call(SCI_POSITIONRELATIVECODEUNITS, static_cast<WPARAM>(pos), static_cast<LPARAM>(relative)));
}

void CScintillaCtrl::CopyRange(_In_ Sci_Position start, _In_ Sci_Position end)
{
  Call(SCI_COPYRANGE, static_cast<WPARAM>(start), static_cast<LPARAM>(end));
}

void CScintillaCtrl::CopyText(_In_ Sci_Position length, _In_reads_bytes_(length) const char* text)
{
  Call(SCI_COPYTEXT, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
}

void CScintillaCtrl::SetSelectionMode(_In_ int selectionMode)
{
  Call(SCI_SETSELECTIONMODE, static_cast<WPARAM>(selectionMode), 0);
}

int CScintillaCtrl::GetSelectionMode()
{
  return static_cast<int>(Call(SCI_GETSELECTIONMODE, 0, 0));
}

BOOL CScintillaCtrl::GetMoveExtendsSelection()
{
  return static_cast<BOOL>(Call(SCI_GETMOVEEXTENDSSELECTION, 0, 0));
}

Sci_Position CScintillaCtrl::GetLineSelStartPosition(_In_ intptr_t line)
{
  return static_cast<Sci_Position>(Call(SCI_GETLINESELSTARTPOSITION, static_cast<WPARAM>(line), 0));
}

Sci_Position CScintillaCtrl::GetLineSelEndPosition(_In_ intptr_t line)
{
  return static_cast<Sci_Position>(Call(SCI_GETLINESELENDPOSITION, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::LineDownRectExtend()
{
  Call(SCI_LINEDOWNRECTEXTEND, 0, 0);
}

void CScintillaCtrl::LineUpRectExtend()
{
  Call(SCI_LINEUPRECTEXTEND, 0, 0);
}

void CScintillaCtrl::CharLeftRectExtend()
{
  Call(SCI_CHARLEFTRECTEXTEND, 0, 0);
}

void CScintillaCtrl::CharRightRectExtend()
{
  Call(SCI_CHARRIGHTRECTEXTEND, 0, 0);
}

void CScintillaCtrl::HomeRectExtend()
{
  Call(SCI_HOMERECTEXTEND, 0, 0);
}

void CScintillaCtrl::VCHomeRectExtend()
{
  Call(SCI_VCHOMERECTEXTEND, 0, 0);
}

void CScintillaCtrl::LineEndRectExtend()
{
  Call(SCI_LINEENDRECTEXTEND, 0, 0);
}

void CScintillaCtrl::PageUpRectExtend()
{
  Call(SCI_PAGEUPRECTEXTEND, 0, 0);
}

void CScintillaCtrl::PageDownRectExtend()
{
  Call(SCI_PAGEDOWNRECTEXTEND, 0, 0);
}

void CScintillaCtrl::StutteredPageUp()
{
  Call(SCI_STUTTEREDPAGEUP, 0, 0);
}

void CScintillaCtrl::StutteredPageUpExtend()
{
  Call(SCI_STUTTEREDPAGEUPEXTEND, 0, 0);
}

void CScintillaCtrl::StutteredPageDown()
{
  Call(SCI_STUTTEREDPAGEDOWN, 0, 0);
}

void CScintillaCtrl::StutteredPageDownExtend()
{
  Call(SCI_STUTTEREDPAGEDOWNEXTEND, 0, 0);
}

void CScintillaCtrl::WordLeftEnd()
{
  Call(SCI_WORDLEFTEND, 0, 0);
}

void CScintillaCtrl::WordLeftEndExtend()
{
  Call(SCI_WORDLEFTENDEXTEND, 0, 0);
}

void CScintillaCtrl::WordRightEnd()
{
  Call(SCI_WORDRIGHTEND, 0, 0);
}

void CScintillaCtrl::WordRightEndExtend()
{
  Call(SCI_WORDRIGHTENDEXTEND, 0, 0);
}

void CScintillaCtrl::SetWhitespaceChars(_In_z_ const char* characters)
{
  Call(SCI_SETWHITESPACECHARS, 0, reinterpret_cast<LPARAM>(characters));
}

int CScintillaCtrl::GetWhitespaceChars(_Inout_opt_z_ char* characters)
{
  return static_cast<int>(Call(SCI_GETWHITESPACECHARS, 0, reinterpret_cast<LPARAM>(characters)));
}

void CScintillaCtrl::SetPunctuationChars(_In_z_ const char* characters)
{
  Call(SCI_SETPUNCTUATIONCHARS, 0, reinterpret_cast<LPARAM>(characters));
}

int CScintillaCtrl::GetPunctuationChars(_Inout_opt_z_ char* characters)
{
  return static_cast<int>(Call(SCI_GETPUNCTUATIONCHARS, 0, reinterpret_cast<LPARAM>(characters)));
}

void CScintillaCtrl::SetCharsDefault()
{
  Call(SCI_SETCHARSDEFAULT, 0, 0);
}

int CScintillaCtrl::AutoCGetCurrent()
{
  return static_cast<int>(Call(SCI_AUTOCGETCURRENT, 0, 0));
}

int CScintillaCtrl::AutoCGetCurrentText(_Inout_opt_z_ char* text)
{
  return static_cast<int>(Call(SCI_AUTOCGETCURRENTTEXT, 0, reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::AutoCSetCaseInsensitiveBehaviour(_In_ int behaviour)
{
  Call(SCI_AUTOCSETCASEINSENSITIVEBEHAVIOUR, static_cast<WPARAM>(behaviour), 0);
}

int CScintillaCtrl::AutoCGetCaseInsensitiveBehaviour()
{
  return static_cast<int>(Call(SCI_AUTOCGETCASEINSENSITIVEBEHAVIOUR, 0, 0));
}

void CScintillaCtrl::AutoCSetMulti(_In_ int multi)
{
  Call(SCI_AUTOCSETMULTI, static_cast<WPARAM>(multi), 0);
}

int CScintillaCtrl::AutoCGetMulti()
{
  return static_cast<int>(Call(SCI_AUTOCGETMULTI, 0, 0));
}

void CScintillaCtrl::AutoCSetOrder(_In_ int order)
{
  Call(SCI_AUTOCSETORDER, static_cast<WPARAM>(order), 0);
}

int CScintillaCtrl::AutoCGetOrder()
{
  return static_cast<int>(Call(SCI_AUTOCGETORDER, 0, 0));
}

void CScintillaCtrl::Allocate(_In_ Sci_Position bytes)
{
  Call(SCI_ALLOCATE, static_cast<WPARAM>(bytes), 0);
}

Sci_Position CScintillaCtrl::TargetAsUTF8(_Inout_ char* s)
{
  return static_cast<Sci_Position>(Call(SCI_TARGETASUTF8, 0, reinterpret_cast<LPARAM>(s)));
}

void CScintillaCtrl::SetLengthForEncode(_In_ Sci_Position bytes)
{
  Call(SCI_SETLENGTHFORENCODE, static_cast<WPARAM>(bytes), 0);
}

Sci_Position CScintillaCtrl::EncodedFromUTF8(_In_ const char* utf8, _Inout_ char* encoded)
{
  return static_cast<Sci_Position>(Call(SCI_ENCODEDFROMUTF8, reinterpret_cast<WPARAM>(utf8), reinterpret_cast<LPARAM>(encoded)));
}

Sci_Position CScintillaCtrl::FindColumn(_In_ intptr_t line, _In_ Sci_Position column)
{
  return static_cast<Sci_Position>(Call(SCI_FINDCOLUMN, static_cast<WPARAM>(line), static_cast<LPARAM>(column)));
}

int CScintillaCtrl::GetCaretSticky()
{
  return static_cast<int>(Call(SCI_GETCARETSTICKY, 0, 0));
}

void CScintillaCtrl::SetCaretSticky(_In_ int useCaretStickyBehaviour)
{
  Call(SCI_SETCARETSTICKY, static_cast<WPARAM>(useCaretStickyBehaviour), 0);
}

void CScintillaCtrl::ToggleCaretSticky()
{
  Call(SCI_TOGGLECARETSTICKY, 0, 0);
}

void CScintillaCtrl::SetPasteConvertEndings(_In_ BOOL convert)
{
  Call(SCI_SETPASTECONVERTENDINGS, static_cast<WPARAM>(convert), 0);
}

BOOL CScintillaCtrl::GetPasteConvertEndings()
{
  return static_cast<BOOL>(Call(SCI_GETPASTECONVERTENDINGS, 0, 0));
}

void CScintillaCtrl::ReplaceRectangular(_In_ Sci_Position length, _In_z_ const char* text)
{
  Call(SCI_REPLACERECTANGULAR, static_cast<WPARAM>(length), reinterpret_cast<LPARAM>(text));
}

void CScintillaCtrl::SelectionDuplicate()
{
  Call(SCI_SELECTIONDUPLICATE, 0, 0);
}

void CScintillaCtrl::SetCaretLineBackAlpha(_In_ int alpha)
{
  Call(SCI_SETCARETLINEBACKALPHA, static_cast<WPARAM>(alpha), 0);
}

int CScintillaCtrl::GetCaretLineBackAlpha()
{
  return static_cast<int>(Call(SCI_GETCARETLINEBACKALPHA, 0, 0));
}

void CScintillaCtrl::SetCaretStyle(_In_ int caretStyle)
{
  Call(SCI_SETCARETSTYLE, static_cast<WPARAM>(caretStyle), 0);
}

int CScintillaCtrl::GetCaretStyle()
{
  return static_cast<int>(Call(SCI_GETCARETSTYLE, 0, 0));
}

void CScintillaCtrl::SetIndicatorCurrent(_In_ int indicator)
{
  Call(SCI_SETINDICATORCURRENT, static_cast<WPARAM>(indicator), 0);
}

int CScintillaCtrl::GetIndicatorCurrent()
{
  return static_cast<int>(Call(SCI_GETINDICATORCURRENT, 0, 0));
}

void CScintillaCtrl::SetIndicatorValue(_In_ int value)
{
  Call(SCI_SETINDICATORVALUE, static_cast<WPARAM>(value), 0);
}

int CScintillaCtrl::GetIndicatorValue()
{
  return static_cast<int>(Call(SCI_GETINDICATORVALUE, 0, 0));
}

void CScintillaCtrl::IndicatorFillRange(_In_ Sci_Position start, _In_ Sci_Position lengthFill)
{
  Call(SCI_INDICATORFILLRANGE, static_cast<WPARAM>(start), static_cast<LPARAM>(lengthFill));
}

void CScintillaCtrl::IndicatorClearRange(_In_ Sci_Position start, _In_ Sci_Position lengthClear)
{
  Call(SCI_INDICATORCLEARRANGE, static_cast<WPARAM>(start), static_cast<LPARAM>(lengthClear));
}

int CScintillaCtrl::IndicatorAllOnFor(_In_ Sci_Position pos)
{
  return static_cast<int>(Call(SCI_INDICATORALLONFOR, static_cast<WPARAM>(pos), 0));
}

int CScintillaCtrl::IndicatorValueAt(_In_ int indicator, _In_ Sci_Position pos)
{
  return static_cast<int>(Call(SCI_INDICATORVALUEAT, static_cast<WPARAM>(indicator), static_cast<LPARAM>(pos)));
}

Sci_Position CScintillaCtrl::IndicatorStart(_In_ int indicator, _In_ Sci_Position pos)
{
  return static_cast<Sci_Position>(Call(SCI_INDICATORSTART, static_cast<WPARAM>(indicator), static_cast<LPARAM>(pos)));
}

Sci_Position CScintillaCtrl::IndicatorEnd(_In_ int indicator, _In_ Sci_Position pos)
{
  return static_cast<Sci_Position>(Call(SCI_INDICATOREND, static_cast<WPARAM>(indicator), static_cast<LPARAM>(pos)));
}

void CScintillaCtrl::SetPositionCache(_In_ int size)
{
  Call(SCI_SETPOSITIONCACHE, static_cast<WPARAM>(size), 0);
}

int CScintillaCtrl::GetPositionCache()
{
  return static_cast<int>(Call(SCI_GETPOSITIONCACHE, 0, 0));
}

void CScintillaCtrl::SetLayoutThreads(_In_ int threads)
{
  Call(SCI_SETLAYOUTTHREADS, static_cast<WPARAM>(threads), 0);
}

int CScintillaCtrl::GetLayoutThreads()
{
  return static_cast<int>(Call(SCI_GETLAYOUTTHREADS, 0, 0));
}

void CScintillaCtrl::CopyAllowLine()
{
  Call(SCI_COPYALLOWLINE, 0, 0);
}

const char* CScintillaCtrl::GetCharacterPointer()
{
#pragma warning(suppress: 26487)
  return reinterpret_cast<const char*>(Call(SCI_GETCHARACTERPOINTER, 0, 0));
}

void* CScintillaCtrl::GetRangePointer(_In_ Sci_Position start, _In_ Sci_Position lengthRange)
{
#pragma warning(suppress: 26487)
  return reinterpret_cast<void*>(Call(SCI_GETRANGEPOINTER, static_cast<WPARAM>(start), static_cast<LPARAM>(lengthRange)));
}

Sci_Position CScintillaCtrl::GetGapPosition()
{
  return static_cast<Sci_Position>(Call(SCI_GETGAPPOSITION, 0, 0));
}

void CScintillaCtrl::IndicSetAlpha(_In_ int indicator, _In_ int alpha)
{
  Call(SCI_INDICSETALPHA, static_cast<WPARAM>(indicator), static_cast<LPARAM>(alpha));
}

int CScintillaCtrl::IndicGetAlpha(_In_ int indicator)
{
  return static_cast<int>(Call(SCI_INDICGETALPHA, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::IndicSetOutlineAlpha(_In_ int indicator, _In_ int alpha)
{
  Call(SCI_INDICSETOUTLINEALPHA, static_cast<WPARAM>(indicator), static_cast<LPARAM>(alpha));
}

int CScintillaCtrl::IndicGetOutlineAlpha(_In_ int indicator)
{
  return static_cast<int>(Call(SCI_INDICGETOUTLINEALPHA, static_cast<WPARAM>(indicator), 0));
}

void CScintillaCtrl::SetExtraAscent(_In_ int extraAscent)
{
  Call(SCI_SETEXTRAASCENT, static_cast<WPARAM>(extraAscent), 0);
}

int CScintillaCtrl::GetExtraAscent()
{
  return static_cast<int>(Call(SCI_GETEXTRAASCENT, 0, 0));
}

void CScintillaCtrl::SetExtraDescent(_In_ int extraDescent)
{
  Call(SCI_SETEXTRADESCENT, static_cast<WPARAM>(extraDescent), 0);
}

int CScintillaCtrl::GetExtraDescent()
{
  return static_cast<int>(Call(SCI_GETEXTRADESCENT, 0, 0));
}

int CScintillaCtrl::MarkerSymbolDefined(_In_ int markerNumber)
{
  return static_cast<int>(Call(SCI_MARKERSYMBOLDEFINED, static_cast<WPARAM>(markerNumber), 0));
}

void CScintillaCtrl::MarginSetText(_In_ intptr_t line, _In_z_ const char* text)
{
  Call(SCI_MARGINSETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
}

int CScintillaCtrl::MarginGetText(_In_ intptr_t line, _Inout_opt_z_ char* text)
{
  return static_cast<int>(Call(SCI_MARGINGETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::MarginSetStyle(_In_ intptr_t line, _In_ int style)
{
  Call(SCI_MARGINSETSTYLE, static_cast<WPARAM>(line), static_cast<LPARAM>(style));
}

int CScintillaCtrl::MarginGetStyle(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_MARGINGETSTYLE, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::MarginSetStyles(_In_ intptr_t line, _In_z_ const char* styles)
{
  Call(SCI_MARGINSETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles));
}

int CScintillaCtrl::MarginGetStyles(_In_ intptr_t line, _Inout_opt_z_ char* styles)
{
  return static_cast<int>(Call(SCI_MARGINGETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles)));
}

void CScintillaCtrl::MarginTextClearAll()
{
  Call(SCI_MARGINTEXTCLEARALL, 0, 0);
}

void CScintillaCtrl::MarginSetStyleOffset(_In_ int style)
{
  Call(SCI_MARGINSETSTYLEOFFSET, static_cast<WPARAM>(style), 0);
}

int CScintillaCtrl::MarginGetStyleOffset()
{
  return static_cast<int>(Call(SCI_MARGINGETSTYLEOFFSET, 0, 0));
}

void CScintillaCtrl::SetMarginOptions(_In_ int marginOptions)
{
  Call(SCI_SETMARGINOPTIONS, static_cast<WPARAM>(marginOptions), 0);
}

int CScintillaCtrl::GetMarginOptions()
{
  return static_cast<int>(Call(SCI_GETMARGINOPTIONS, 0, 0));
}

void CScintillaCtrl::AnnotationSetText(_In_ intptr_t line, _In_z_ const char* text)
{
  Call(SCI_ANNOTATIONSETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
}

int CScintillaCtrl::AnnotationGetText(_In_ intptr_t line, _Inout_opt_z_ char* text)
{
  return static_cast<int>(Call(SCI_ANNOTATIONGETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::AnnotationSetStyle(_In_ intptr_t line, _In_ int style)
{
  Call(SCI_ANNOTATIONSETSTYLE, static_cast<WPARAM>(line), static_cast<LPARAM>(style));
}

int CScintillaCtrl::AnnotationGetStyle(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_ANNOTATIONGETSTYLE, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::AnnotationSetStyles(_In_ intptr_t line, _In_z_ const char* styles)
{
  Call(SCI_ANNOTATIONSETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles));
}

int CScintillaCtrl::AnnotationGetStyles(_In_ intptr_t line, _Inout_opt_z_ char* styles)
{
  return static_cast<int>(Call(SCI_ANNOTATIONGETSTYLES, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(styles)));
}

int CScintillaCtrl::AnnotationGetLines(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_ANNOTATIONGETLINES, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::AnnotationClearAll()
{
  Call(SCI_ANNOTATIONCLEARALL, 0, 0);
}

void CScintillaCtrl::AnnotationSetVisible(_In_ int visible)
{
  Call(SCI_ANNOTATIONSETVISIBLE, static_cast<WPARAM>(visible), 0);
}

int CScintillaCtrl::AnnotationGetVisible()
{
  return static_cast<int>(Call(SCI_ANNOTATIONGETVISIBLE, 0, 0));
}

void CScintillaCtrl::AnnotationSetStyleOffset(_In_ int style)
{
  Call(SCI_ANNOTATIONSETSTYLEOFFSET, static_cast<WPARAM>(style), 0);
}

int CScintillaCtrl::AnnotationGetStyleOffset()
{
  return static_cast<int>(Call(SCI_ANNOTATIONGETSTYLEOFFSET, 0, 0));
}

void CScintillaCtrl::ReleaseAllExtendedStyles()
{
  Call(SCI_RELEASEALLEXTENDEDSTYLES, 0, 0);
}

int CScintillaCtrl::AllocateExtendedStyles(_In_ int numberStyles)
{
  return static_cast<int>(Call(SCI_ALLOCATEEXTENDEDSTYLES, static_cast<WPARAM>(numberStyles), 0));
}

void CScintillaCtrl::AddUndoAction(_In_ int token, _In_ int flags)
{
  Call(SCI_ADDUNDOACTION, static_cast<WPARAM>(token), static_cast<LPARAM>(flags));
}

Sci_Position CScintillaCtrl::CharPositionFromPoint(_In_ int x, _In_ int y)
{
  return static_cast<Sci_Position>(Call(SCI_CHARPOSITIONFROMPOINT, static_cast<WPARAM>(x), static_cast<LPARAM>(y)));
}

Sci_Position CScintillaCtrl::CharPositionFromPointClose(_In_ int x, _In_ int y)
{
  return static_cast<Sci_Position>(Call(SCI_CHARPOSITIONFROMPOINTCLOSE, static_cast<WPARAM>(x), static_cast<LPARAM>(y)));
}

void CScintillaCtrl::SetMouseSelectionRectangularSwitch(_In_ BOOL mouseSelectionRectangularSwitch)
{
  Call(SCI_SETMOUSESELECTIONRECTANGULARSWITCH, static_cast<WPARAM>(mouseSelectionRectangularSwitch), 0);
}

BOOL CScintillaCtrl::GetMouseSelectionRectangularSwitch()
{
  return static_cast<BOOL>(Call(SCI_GETMOUSESELECTIONRECTANGULARSWITCH, 0, 0));
}

void CScintillaCtrl::SetMultipleSelection(_In_ BOOL multipleSelection)
{
  Call(SCI_SETMULTIPLESELECTION, static_cast<WPARAM>(multipleSelection), 0);
}

BOOL CScintillaCtrl::GetMultipleSelection()
{
  return static_cast<BOOL>(Call(SCI_GETMULTIPLESELECTION, 0, 0));
}

void CScintillaCtrl::SetAdditionalSelectionTyping(_In_ BOOL additionalSelectionTyping)
{
  Call(SCI_SETADDITIONALSELECTIONTYPING, static_cast<WPARAM>(additionalSelectionTyping), 0);
}

BOOL CScintillaCtrl::GetAdditionalSelectionTyping()
{
  return static_cast<BOOL>(Call(SCI_GETADDITIONALSELECTIONTYPING, 0, 0));
}

void CScintillaCtrl::SetAdditionalCaretsBlink(_In_ BOOL additionalCaretsBlink)
{
  Call(SCI_SETADDITIONALCARETSBLINK, static_cast<WPARAM>(additionalCaretsBlink), 0);
}

BOOL CScintillaCtrl::GetAdditionalCaretsBlink()
{
  return static_cast<BOOL>(Call(SCI_GETADDITIONALCARETSBLINK, 0, 0));
}

void CScintillaCtrl::SetAdditionalCaretsVisible(_In_ BOOL additionalCaretsVisible)
{
  Call(SCI_SETADDITIONALCARETSVISIBLE, static_cast<WPARAM>(additionalCaretsVisible), 0);
}

BOOL CScintillaCtrl::GetAdditionalCaretsVisible()
{
  return static_cast<BOOL>(Call(SCI_GETADDITIONALCARETSVISIBLE, 0, 0));
}

int CScintillaCtrl::GetSelections()
{
  return static_cast<int>(Call(SCI_GETSELECTIONS, 0, 0));
}

BOOL CScintillaCtrl::GetSelectionEmpty()
{
  return static_cast<BOOL>(Call(SCI_GETSELECTIONEMPTY, 0, 0));
}

void CScintillaCtrl::ClearSelections()
{
  Call(SCI_CLEARSELECTIONS, 0, 0);
}

void CScintillaCtrl::SetSelection(_In_ Sci_Position caret, _In_ Sci_Position anchor)
{
  Call(SCI_SETSELECTION, static_cast<WPARAM>(caret), static_cast<LPARAM>(anchor));
}

void CScintillaCtrl::AddSelection(_In_ Sci_Position caret, _In_ Sci_Position anchor)
{
  Call(SCI_ADDSELECTION, static_cast<WPARAM>(caret), static_cast<LPARAM>(anchor));
}

void CScintillaCtrl::DropSelectionN(_In_ int selection)
{
  Call(SCI_DROPSELECTIONN, static_cast<WPARAM>(selection), 0);
}

void CScintillaCtrl::SetMainSelection(_In_ int selection)
{
  Call(SCI_SETMAINSELECTION, static_cast<WPARAM>(selection), 0);
}

int CScintillaCtrl::GetMainSelection()
{
  return static_cast<int>(Call(SCI_GETMAINSELECTION, 0, 0));
}

void CScintillaCtrl::SetSelectionNCaret(_In_ int selection, _In_ Sci_Position caret)
{
  Call(SCI_SETSELECTIONNCARET, static_cast<WPARAM>(selection), static_cast<LPARAM>(caret));
}

Sci_Position CScintillaCtrl::GetSelectionNCaret(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNCARET, static_cast<WPARAM>(selection), 0));
}

void CScintillaCtrl::SetSelectionNAnchor(_In_ int selection, _In_ Sci_Position anchor)
{
  Call(SCI_SETSELECTIONNANCHOR, static_cast<WPARAM>(selection), static_cast<LPARAM>(anchor));
}

Sci_Position CScintillaCtrl::GetSelectionNAnchor(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNANCHOR, static_cast<WPARAM>(selection), 0));
}

void CScintillaCtrl::SetSelectionNCaretVirtualSpace(_In_ int selection, _In_ Sci_Position space)
{
  Call(SCI_SETSELECTIONNCARETVIRTUALSPACE, static_cast<WPARAM>(selection), static_cast<LPARAM>(space));
}

Sci_Position CScintillaCtrl::GetSelectionNCaretVirtualSpace(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNCARETVIRTUALSPACE, static_cast<WPARAM>(selection), 0));
}

void CScintillaCtrl::SetSelectionNAnchorVirtualSpace(_In_ int selection, _In_ Sci_Position space)
{
  Call(SCI_SETSELECTIONNANCHORVIRTUALSPACE, static_cast<WPARAM>(selection), static_cast<LPARAM>(space));
}

Sci_Position CScintillaCtrl::GetSelectionNAnchorVirtualSpace(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNANCHORVIRTUALSPACE, static_cast<WPARAM>(selection), 0));
}

void CScintillaCtrl::SetSelectionNStart(_In_ int selection, _In_ Sci_Position anchor)
{
  Call(SCI_SETSELECTIONNSTART, static_cast<WPARAM>(selection), static_cast<LPARAM>(anchor));
}

Sci_Position CScintillaCtrl::GetSelectionNStart(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNSTART, static_cast<WPARAM>(selection), 0));
}

Sci_Position CScintillaCtrl::GetSelectionNStartVirtualSpace(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNSTARTVIRTUALSPACE, static_cast<WPARAM>(selection), 0));
}

void CScintillaCtrl::SetSelectionNEnd(_In_ int selection, _In_ Sci_Position caret)
{
  Call(SCI_SETSELECTIONNEND, static_cast<WPARAM>(selection), static_cast<LPARAM>(caret));
}

Sci_Position CScintillaCtrl::GetSelectionNEndVirtualSpace(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNENDVIRTUALSPACE, static_cast<WPARAM>(selection), 0));
}

Sci_Position CScintillaCtrl::GetSelectionNEnd(_In_ int selection)
{
  return static_cast<Sci_Position>(Call(SCI_GETSELECTIONNEND, static_cast<WPARAM>(selection), 0));
}

void CScintillaCtrl::SetRectangularSelectionCaret(_In_ Sci_Position caret)
{
  Call(SCI_SETRECTANGULARSELECTIONCARET, static_cast<WPARAM>(caret), 0);
}

Sci_Position CScintillaCtrl::GetRectangularSelectionCaret()
{
  return static_cast<Sci_Position>(Call(SCI_GETRECTANGULARSELECTIONCARET, 0, 0));
}

void CScintillaCtrl::SetRectangularSelectionAnchor(_In_ Sci_Position anchor)
{
  Call(SCI_SETRECTANGULARSELECTIONANCHOR, static_cast<WPARAM>(anchor), 0);
}

Sci_Position CScintillaCtrl::GetRectangularSelectionAnchor()
{
  return static_cast<Sci_Position>(Call(SCI_GETRECTANGULARSELECTIONANCHOR, 0, 0));
}

void CScintillaCtrl::SetRectangularSelectionCaretVirtualSpace(_In_ Sci_Position space)
{
  Call(SCI_SETRECTANGULARSELECTIONCARETVIRTUALSPACE, static_cast<WPARAM>(space), 0);
}

Sci_Position CScintillaCtrl::GetRectangularSelectionCaretVirtualSpace()
{
  return static_cast<Sci_Position>(Call(SCI_GETRECTANGULARSELECTIONCARETVIRTUALSPACE, 0, 0));
}

void CScintillaCtrl::SetRectangularSelectionAnchorVirtualSpace(_In_ Sci_Position space)
{
  Call(SCI_SETRECTANGULARSELECTIONANCHORVIRTUALSPACE, static_cast<WPARAM>(space), 0);
}

Sci_Position CScintillaCtrl::GetRectangularSelectionAnchorVirtualSpace()
{
  return static_cast<Sci_Position>(Call(SCI_GETRECTANGULARSELECTIONANCHORVIRTUALSPACE, 0, 0));
}

void CScintillaCtrl::SetVirtualSpaceOptions(_In_ int virtualSpaceOptions)
{
  Call(SCI_SETVIRTUALSPACEOPTIONS, static_cast<WPARAM>(virtualSpaceOptions), 0);
}

int CScintillaCtrl::GetVirtualSpaceOptions()
{
  return static_cast<int>(Call(SCI_GETVIRTUALSPACEOPTIONS, 0, 0));
}

void CScintillaCtrl::SetRectangularSelectionModifier(_In_ int modifier)
{
  Call(SCI_SETRECTANGULARSELECTIONMODIFIER, static_cast<WPARAM>(modifier), 0);
}

int CScintillaCtrl::GetRectangularSelectionModifier()
{
  return static_cast<int>(Call(SCI_GETRECTANGULARSELECTIONMODIFIER, 0, 0));
}

void CScintillaCtrl::SetAdditionalSelFore(_In_ COLORREF fore)
{
  Call(SCI_SETADDITIONALSELFORE, static_cast<WPARAM>(fore), 0);
}

void CScintillaCtrl::SetAdditionalSelBack(_In_ COLORREF back)
{
  Call(SCI_SETADDITIONALSELBACK, static_cast<WPARAM>(back), 0);
}

void CScintillaCtrl::SetAdditionalSelAlpha(_In_ int alpha)
{
  Call(SCI_SETADDITIONALSELALPHA, static_cast<WPARAM>(alpha), 0);
}

int CScintillaCtrl::GetAdditionalSelAlpha()
{
  return static_cast<int>(Call(SCI_GETADDITIONALSELALPHA, 0, 0));
}

void CScintillaCtrl::SetAdditionalCaretFore(_In_ COLORREF fore)
{
  Call(SCI_SETADDITIONALCARETFORE, static_cast<WPARAM>(fore), 0);
}

COLORREF CScintillaCtrl::GetAdditionalCaretFore()
{
  return static_cast<COLORREF>(Call(SCI_GETADDITIONALCARETFORE, 0, 0));
}

void CScintillaCtrl::RotateSelection()
{
  Call(SCI_ROTATESELECTION, 0, 0);
}

void CScintillaCtrl::SwapMainAnchorCaret()
{
  Call(SCI_SWAPMAINANCHORCARET, 0, 0);
}

void CScintillaCtrl::MultipleSelectAddNext()
{
  Call(SCI_MULTIPLESELECTADDNEXT, 0, 0);
}

void CScintillaCtrl::MultipleSelectAddEach()
{
  Call(SCI_MULTIPLESELECTADDEACH, 0, 0);
}

int CScintillaCtrl::ChangeLexerState(_In_ Sci_Position start, _In_ Sci_Position end)
{
  return static_cast<int>(Call(SCI_CHANGELEXERSTATE, static_cast<WPARAM>(start), static_cast<LPARAM>(end)));
}

intptr_t CScintillaCtrl::ContractedFoldNext(_In_ intptr_t lineStart)
{
  return static_cast<intptr_t>(Call(SCI_CONTRACTEDFOLDNEXT, static_cast<WPARAM>(lineStart), 0));
}

void CScintillaCtrl::VerticalCentreCaret()
{
  Call(SCI_VERTICALCENTRECARET, 0, 0);
}

void CScintillaCtrl::MoveSelectedLinesUp()
{
  Call(SCI_MOVESELECTEDLINESUP, 0, 0);
}

void CScintillaCtrl::MoveSelectedLinesDown()
{
  Call(SCI_MOVESELECTEDLINESDOWN, 0, 0);
}

void CScintillaCtrl::SetIdentifier(_In_ int identifier)
{
  Call(SCI_SETIDENTIFIER, static_cast<WPARAM>(identifier), 0);
}

int CScintillaCtrl::GetIdentifier()
{
  return static_cast<int>(Call(SCI_GETIDENTIFIER, 0, 0));
}

void CScintillaCtrl::RGBAImageSetWidth(_In_ int width)
{
  Call(SCI_RGBAIMAGESETWIDTH, static_cast<WPARAM>(width), 0);
}

void CScintillaCtrl::RGBAImageSetHeight(_In_ int height)
{
  Call(SCI_RGBAIMAGESETHEIGHT, static_cast<WPARAM>(height), 0);
}

void CScintillaCtrl::RGBAImageSetScale(_In_ int scalePercent)
{
  Call(SCI_RGBAIMAGESETSCALE, static_cast<WPARAM>(scalePercent), 0);
}

void CScintillaCtrl::MarkerDefineRGBAImage(_In_ int markerNumber, _In_ const char* pixels)
{
  Call(SCI_MARKERDEFINERGBAIMAGE, static_cast<WPARAM>(markerNumber), reinterpret_cast<LPARAM>(pixels));
}

void CScintillaCtrl::RegisterRGBAImage(_In_ int type, _In_ const char* pixels)
{
  Call(SCI_REGISTERRGBAIMAGE, static_cast<WPARAM>(type), reinterpret_cast<LPARAM>(pixels));
}

void CScintillaCtrl::ScrollToStart()
{
  Call(SCI_SCROLLTOSTART, 0, 0);
}

void CScintillaCtrl::ScrollToEnd()
{
  Call(SCI_SCROLLTOEND, 0, 0);
}

void CScintillaCtrl::SetTechnology(_In_ int technology)
{
  Call(SCI_SETTECHNOLOGY, static_cast<WPARAM>(technology), 0);
}

int CScintillaCtrl::GetTechnology()
{
  return static_cast<int>(Call(SCI_GETTECHNOLOGY, 0, 0));
}

void* CScintillaCtrl::CreateLoader(_In_ Sci_Position bytes, _In_ int documentOptions)
{
#pragma warning(suppress: 26487)
  return reinterpret_cast<void*>(Call(SCI_CREATELOADER, static_cast<WPARAM>(bytes), static_cast<LPARAM>(documentOptions)));
}

void CScintillaCtrl::FindIndicatorShow(_In_ Sci_Position start, _In_ Sci_Position end)
{
  Call(SCI_FINDINDICATORSHOW, static_cast<WPARAM>(start), static_cast<LPARAM>(end));
}

void CScintillaCtrl::FindIndicatorFlash(_In_ Sci_Position start, _In_ Sci_Position end)
{
  Call(SCI_FINDINDICATORFLASH, static_cast<WPARAM>(start), static_cast<LPARAM>(end));
}

void CScintillaCtrl::FindIndicatorHide()
{
  Call(SCI_FINDINDICATORHIDE, 0, 0);
}

void CScintillaCtrl::VCHomeDisplay()
{
  Call(SCI_VCHOMEDISPLAY, 0, 0);
}

void CScintillaCtrl::VCHomeDisplayExtend()
{
  Call(SCI_VCHOMEDISPLAYEXTEND, 0, 0);
}

BOOL CScintillaCtrl::GetCaretLineVisibleAlways()
{
  return static_cast<BOOL>(Call(SCI_GETCARETLINEVISIBLEALWAYS, 0, 0));
}

void CScintillaCtrl::SetCaretLineVisibleAlways(_In_ BOOL bAlwaysVisible)
{
  Call(SCI_SETCARETLINEVISIBLEALWAYS, static_cast<WPARAM>(bAlwaysVisible), 0);
}

void CScintillaCtrl::SetLineEndTypesAllowed(_In_ int lineEndBitSet)
{
  Call(SCI_SETLINEENDTYPESALLOWED, static_cast<WPARAM>(lineEndBitSet), 0);
}

int CScintillaCtrl::GetLineEndTypesAllowed()
{
  return static_cast<int>(Call(SCI_GETLINEENDTYPESALLOWED, 0, 0));
}

int CScintillaCtrl::GetLineEndTypesActive()
{
  return static_cast<int>(Call(SCI_GETLINEENDTYPESACTIVE, 0, 0));
}

void CScintillaCtrl::SetRepresentation(_In_z_ const char* encodedCharacter, _In_z_ const char* representation)
{
  Call(SCI_SETREPRESENTATION, reinterpret_cast<WPARAM>(encodedCharacter), reinterpret_cast<LPARAM>(representation));
}

int CScintillaCtrl::GetRepresentation(_In_z_ const char* encodedCharacter, _Inout_z_ char* representation)
{
  return static_cast<int>(Call(SCI_GETREPRESENTATION, reinterpret_cast<WPARAM>(encodedCharacter), reinterpret_cast<LPARAM>(representation)));
}

void CScintillaCtrl::ClearRepresentation(_In_z_ const char* encodedCharacter)
{
  Call(SCI_CLEARREPRESENTATION, reinterpret_cast<WPARAM>(encodedCharacter), 0);
}

void CScintillaCtrl::ClearAllRepresentations()
{
  Call(SCI_CLEARALLREPRESENTATIONS, 0, 0);
}

void CScintillaCtrl::SetRepresentationAppearance(_In_z_ const char* encodedCharacter, _In_ int appearance)
{
  Call(SCI_SETREPRESENTATIONAPPEARANCE, reinterpret_cast<WPARAM>(encodedCharacter), static_cast<LPARAM>(appearance));
}

int CScintillaCtrl::GetRepresentationAppearance(_In_z_ const char* encodedCharacter)
{
  return static_cast<int>(Call(SCI_GETREPRESENTATIONAPPEARANCE, reinterpret_cast<WPARAM>(encodedCharacter), 0));
}

void CScintillaCtrl::SetRepresentationColour(_In_z_ const char* encodedCharacter, _In_ int colour)
{
  Call(SCI_SETREPRESENTATIONCOLOUR, reinterpret_cast<WPARAM>(encodedCharacter), static_cast<LPARAM>(colour));
}

int CScintillaCtrl::GetRepresentationColour(_In_z_ const char* encodedCharacter)
{
  return static_cast<int>(Call(SCI_GETREPRESENTATIONCOLOUR, reinterpret_cast<WPARAM>(encodedCharacter), 0));
}

void CScintillaCtrl::EOLAnnotationSetText(_In_ intptr_t line, _In_z_ const char* text)
{
  Call(SCI_EOLANNOTATIONSETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text));
}

int CScintillaCtrl::EOLAnnotationGetText(_In_ intptr_t line, _Inout_opt_z_ char* text)
{
  return static_cast<int>(Call(SCI_EOLANNOTATIONGETTEXT, static_cast<WPARAM>(line), reinterpret_cast<LPARAM>(text)));
}

void CScintillaCtrl::EOLAnnotationSetStyle(_In_ intptr_t line, _In_ int style)
{
  Call(SCI_EOLANNOTATIONSETSTYLE, static_cast<WPARAM>(line), static_cast<LPARAM>(style));
}

int CScintillaCtrl::EOLAnnotationGetStyle(_In_ intptr_t line)
{
  return static_cast<int>(Call(SCI_EOLANNOTATIONGETSTYLE, static_cast<WPARAM>(line), 0));
}

void CScintillaCtrl::EOLAnnotationClearAll()
{
  Call(SCI_EOLANNOTATIONCLEARALL, 0, 0);
}

void CScintillaCtrl::EOLAnnotationSetVisible(_In_ int visible)
{
  Call(SCI_EOLANNOTATIONSETVISIBLE, static_cast<WPARAM>(visible), 0);
}

int CScintillaCtrl::EOLAnnotationGetVisible()
{
  return static_cast<int>(Call(SCI_EOLANNOTATIONGETVISIBLE, 0, 0));
}

void CScintillaCtrl::EOLAnnotationSetStyleOffset(_In_ int style)
{
  Call(SCI_EOLANNOTATIONSETSTYLEOFFSET, static_cast<WPARAM>(style), 0);
}

int CScintillaCtrl::EOLAnnotationGetStyleOffset()
{
  return static_cast<int>(Call(SCI_EOLANNOTATIONGETSTYLEOFFSET, 0, 0));
}

BOOL CScintillaCtrl::SupportsFeature(_In_ int feature)
{
  return static_cast<BOOL>(Call(SCI_SUPPORTSFEATURE, static_cast<WPARAM>(feature), 0));
}

int CScintillaCtrl::GetLineCharacterIndex()
{
  return static_cast<int>(Call(SCI_GETLINECHARACTERINDEX, 0, 0));
}

void CScintillaCtrl::AllocateLineCharacterIndex(_In_ int lineCharacterIndex)
{
  Call(SCI_ALLOCATELINECHARACTERINDEX, static_cast<WPARAM>(lineCharacterIndex), 0);
}

void CScintillaCtrl::ReleaseLineCharacterIndex(_In_ int lineCharacterIndex)
{
  Call(SCI_RELEASELINECHARACTERINDEX, static_cast<WPARAM>(lineCharacterIndex), 0);
}

intptr_t CScintillaCtrl::LineFromIndexPosition(_In_ Sci_Position pos, _In_ int lineCharacterIndex)
{
  return static_cast<intptr_t>(Call(SCI_LINEFROMINDEXPOSITION, static_cast<WPARAM>(pos), static_cast<LPARAM>(lineCharacterIndex)));
}

Sci_Position CScintillaCtrl::IndexPositionFromLine(_In_ intptr_t line, _In_ int lineCharacterIndex)
{
  return static_cast<Sci_Position>(Call(SCI_INDEXPOSITIONFROMLINE, static_cast<WPARAM>(line), static_cast<LPARAM>(lineCharacterIndex)));
}

void CScintillaCtrl::StartRecord()
{
  Call(SCI_STARTRECORD, 0, 0);
}

void CScintillaCtrl::StopRecord()
{
  Call(SCI_STOPRECORD, 0, 0);
}

int CScintillaCtrl::GetLexer()
{
  return static_cast<int>(Call(SCI_GETLEXER, 0, 0));
}

void CScintillaCtrl::Colourise(_In_ Sci_Position start, _In_ Sci_Position end)
{
  Call(SCI_COLOURISE, static_cast<WPARAM>(start), static_cast<LPARAM>(end));
}

void CScintillaCtrl::SetScintillaProperty(_In_z_ const char* key, _In_z_ const char* value)
{
  Call(SCI_SETPROPERTY, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(value));
}

void CScintillaCtrl::SetKeyWords(_In_ int keyWordSet, _In_z_ const char* keyWords)
{
  Call(SCI_SETKEYWORDS, static_cast<WPARAM>(keyWordSet), reinterpret_cast<LPARAM>(keyWords));
}

int CScintillaCtrl::GetScintillaProperty(_In_z_ const char* key, _Inout_opt_z_ char* value)
{
  return static_cast<int>(Call(SCI_GETPROPERTY, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(value)));
}

int CScintillaCtrl::GetPropertyExpanded(_In_z_ const char* key, _Inout_opt_z_ char* value)
{
  return static_cast<int>(Call(SCI_GETPROPERTYEXPANDED, reinterpret_cast<WPARAM>(key), reinterpret_cast<LPARAM>(value)));
}

int CScintillaCtrl::GetPropertyInt(_In_z_ const char* key, _In_ int defaultValue)
{
  return static_cast<int>(Call(SCI_GETPROPERTYINT, reinterpret_cast<WPARAM>(key), defaultValue));
}

int CScintillaCtrl::GetLexerLanguage(_Inout_opt_z_ char* language)
{
  return static_cast<int>(Call(SCI_GETLEXERLANGUAGE, 0, reinterpret_cast<LPARAM>(language)));
}

void* CScintillaCtrl::PrivateLexerCall(_In_ int operation, _In_opt_ void* pointer)
{
#pragma warning(suppress: 26487)
  return reinterpret_cast<void*>(Call(SCI_PRIVATELEXERCALL, static_cast<WPARAM>(operation), reinterpret_cast<LPARAM>(pointer)));
}

int CScintillaCtrl::PropertyNames(_Inout_opt_z_ char* names)
{
  return static_cast<int>(Call(SCI_PROPERTYNAMES, 0, reinterpret_cast<LPARAM>(names)));
}

int CScintillaCtrl::PropertyType(_In_z_ const char* name)
{
  return static_cast<int>(Call(SCI_PROPERTYTYPE, reinterpret_cast<WPARAM>(name), 0));
}

int CScintillaCtrl::DescribeProperty(_In_z_ const char* name, _Inout_opt_z_ char* description)
{
  return static_cast<int>(Call(SCI_DESCRIBEPROPERTY, reinterpret_cast<WPARAM>(name), reinterpret_cast<LPARAM>(description)));
}

int CScintillaCtrl::DescribeKeyWordSets(_Inout_opt_z_ char* descriptions)
{
  return static_cast<int>(Call(SCI_DESCRIBEKEYWORDSETS, 0, reinterpret_cast<LPARAM>(descriptions)));
}

int CScintillaCtrl::GetLineEndTypesSupported()
{
  return static_cast<int>(Call(SCI_GETLINEENDTYPESSUPPORTED, 0, 0));
}

int CScintillaCtrl::AllocateSubStyles(_In_ int styleBase, _In_ int numberStyles)
{
  return static_cast<int>(Call(SCI_ALLOCATESUBSTYLES, static_cast<WPARAM>(styleBase), static_cast<LPARAM>(numberStyles)));
}

int CScintillaCtrl::GetSubStylesStart(_In_ int styleBase)
{
  return static_cast<int>(Call(SCI_GETSUBSTYLESSTART, static_cast<WPARAM>(styleBase), 0));
}

int CScintillaCtrl::GetSubStylesLength(_In_ int styleBase)
{
  return static_cast<int>(Call(SCI_GETSUBSTYLESLENGTH, static_cast<WPARAM>(styleBase), 0));
}

int CScintillaCtrl::GetStyleFromSubStyle(_In_ int subStyle)
{
  return static_cast<int>(Call(SCI_GETSTYLEFROMSUBSTYLE, static_cast<WPARAM>(subStyle), 0));
}

int CScintillaCtrl::GetPrimaryStyleFromStyle(_In_ int style)
{
  return static_cast<int>(Call(SCI_GETPRIMARYSTYLEFROMSTYLE, static_cast<WPARAM>(style), 0));
}

void CScintillaCtrl::FreeSubStyles()
{
  Call(SCI_FREESUBSTYLES, 0, 0);
}

void CScintillaCtrl::SetIdentifiers(_In_ int style, _In_z_ const char* identifiers)
{
  Call(SCI_SETIDENTIFIERS, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(identifiers));
}

int CScintillaCtrl::DistanceToSecondaryStyles()
{
  return static_cast<int>(Call(SCI_DISTANCETOSECONDARYSTYLES, 0, 0));
}

int CScintillaCtrl::GetSubStyleBases(_Inout_opt_z_ char* styles)
{
  return static_cast<int>(Call(SCI_GETSUBSTYLEBASES, 0, reinterpret_cast<LPARAM>(styles)));
}

int CScintillaCtrl::GetNamedStyles()
{
  return static_cast<int>(Call(SCI_GETNAMEDSTYLES, 0, 0));
}

int CScintillaCtrl::NameOfStyle(_In_ int style, _Inout_opt_z_ char* name)
{
  return static_cast<int>(Call(SCI_NAMEOFSTYLE, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(name)));
}

int CScintillaCtrl::TagsOfStyle(_In_ int style, _Inout_opt_z_ char* tags)
{
  return static_cast<int>(Call(SCI_TAGSOFSTYLE, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(tags)));
}

int CScintillaCtrl::DescriptionOfStyle(_In_ int style, _Inout_opt_z_ char* description)
{
  return static_cast<int>(Call(SCI_DESCRIPTIONOFSTYLE, static_cast<WPARAM>(style), reinterpret_cast<LPARAM>(description)));
}

void CScintillaCtrl::SetILexer(_In_ void* ilexer)
{
  Call(SCI_SETILEXER, 0, reinterpret_cast<LPARAM>(ilexer));
}

int CScintillaCtrl::GetBidirectional()
{
  return static_cast<int>(Call(SCI_GETBIDIRECTIONAL, 0, 0));
}

void CScintillaCtrl::SetBidirectional(_In_ int bidirectional)
{
  Call(SCI_SETBIDIRECTIONAL, static_cast<WPARAM>(bidirectional), 0);
}
#pragma warning(pop)
