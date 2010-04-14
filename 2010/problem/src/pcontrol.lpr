program pcontrol;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, main, LResources, map, playerframe, aihandler, log
  { you can add units after this };

{$IFDEF WINDOWS}{$R pcontrol.rc}{$ENDIF}

begin
  {$I pcontrol.lrs}
  Application.Initialize;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.

