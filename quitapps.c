/*	File:		quitapps.c	Contains:	DTS Code Snippet to quit all running applications (except yourself)					note: to work properly, the calling application must have a standard				event loop with menus (to support puppet string quits for apps that				don't support core appleevents.								note#2: remember to set the applevent aware flag in your app if you use				this code	Written by: Steven Falkenburg 		Copyright:	Copyright � 1991-1999 by Apple Computer, Inc., All Rights Reserved.				You may incorporate this Apple sample source code into your program(s) without				restriction. This Apple sample source code has been provided "AS IS" and the				responsibility for its operation is yours. You are not permitted to redistribute				this Apple sample source code as "Apple sample source code" after having made				changes. If you're going to re-distribute the source, we require that you make				it clear in the source that the code was descended from Apple sample source				code, but that you've made changes.	Change History (most recent first):				7/27/1999	Karl Groethe	Updated for Metrowerks Codewarror Pro 2.1				*/#include <Processes.h>#include <AppleEvents.h>#include <Quickdraw.h>#include <Fonts.h>#include <Windows.h>#include <TextEdit.h>#include <Dialogs.h>/* protos */void QuitAllApps(void);OSErr QuitAnApp(ProcessSerialNumber *proc);short GetNumProcs(void);void main(){	    InitGraf(&qd.thePort);    InitFonts();    InitWindows();    InitMenus();    TEInit();    InitDialogs(nil);    InitCursor();        	QuitAllApps();}/* this is the entry procedure.  call this to start things going */void QuitAllApps(void){	ProcessSerialNumber ourProc,curProc,*quitApps;	OSErr err;	Boolean same;	short numApps,curAppIndex;		numApps = GetNumProcs() - 1; /* don't count ourselves */	quitApps = (ProcessSerialNumber *)NewPtr(numApps*sizeof(ProcessSerialNumber));	if (MemError()!=noErr)		return;			err = GetCurrentProcess(&ourProc);	if (err!=noErr)		return;		curProc.highLongOfPSN = 0;	curProc.lowLongOfPSN = kNoProcess;	curAppIndex = 0;		while (GetNextProcess(&curProc)==noErr && curAppIndex<numApps) {		err = SameProcess(&ourProc,&curProc,&same);		if (err==noErr && !same) {			BlockMove(&curProc,&quitApps[curAppIndex++],sizeof(ProcessSerialNumber));		}	}		for (curAppIndex=0; curAppIndex<numApps; curAppIndex++)		QuitAnApp(&quitApps[curAppIndex]);		DisposePtr((Ptr)quitApps);}/* gets the number of current processes */short GetNumProcs(void){	ProcessSerialNumber curProc;	short numProcs;		numProcs = 0;	curProc.highLongOfPSN = 0;	curProc.lowLongOfPSN = kNoProcess;		while (GetNextProcess(&curProc)==noErr)		numProcs++;		return numProcs;}/* quits an app of the given process id */OSErr QuitAnApp(ProcessSerialNumber *proc){	OSErr err;	AEAddressDesc target;	AppleEvent theAE,aeReply;		theAE.dataHandle = aeReply.dataHandle = target.dataHandle = nil;		err = AECreateDesc(typeProcessSerialNumber,(Ptr)proc,sizeof(ProcessSerialNumber),&target);	if (err!=noErr)		return err;	err = AECreateAppleEvent(kCoreEventClass,kAEQuitApplication,&target,					kAutoGenerateReturnID,kAnyTransactionID,&theAE);	if (err!=noErr) {		AEDisposeDesc(&target);		return err;	}	 	err = AESend(&theAE,&aeReply,kAENoReply,kAENormalPriority,kNoTimeOut,nil,nil);			AEDisposeDesc(&target);	AEDisposeDesc(&theAE);		return err;}