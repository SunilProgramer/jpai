program Pipes;

{$mode objfpc}{$H+}

uses
  {$IFDEF UNIX}{$IFDEF UseCThreads}
  cthreads,
  {$ENDIF}{$ENDIF}
  Interfaces, // this includes the LCL widgetset
  Forms, LResources, main, map, PlayerFrame;

{$IFDEF WINDOWS}{$R Pipes.rc}{$ENDIF}

begin
  {$I Pipes.lrs}
  Application.Initialize;
  Application.CreateForm(TfrmMain, frmMain);
  Application.Run;
end.

