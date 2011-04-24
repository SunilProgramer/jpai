program pcontrol;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, main, LResources, map, playerframe, aihandler, log, intdialog
  { you can add units after this };

{$IFDEF WINDOWS}{$R pcontrol.rc}{$ENDIF}

begin
  {$I pcontrol.lrs}
  Application.Initialize;
  Application.CreateForm(TfrmMain, frmMain);
  Application.CreateForm(TfrmIntDialog, frmIntDialog);
  Application.Run;
end.

