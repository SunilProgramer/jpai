unit intdialog;

{$mode objfpc}

interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, Controls, Graphics, Dialogs,
  StdCtrls, Spin;

type

  { TfrmIntDialog }

  TfrmIntDialog = class(TForm)
    Button1: TButton;
    Button2: TButton;
    seVal: TSpinEdit;
    procedure FormClose(Sender: TObject; var CloseAction: TCloseAction);
  private
    { private declarations }
  public
    { public declarations }
  end; 

var
  frmIntDialog: TfrmIntDialog;

implementation

{ TfrmIntDialog }

procedure TfrmIntDialog.FormClose(Sender: TObject; var CloseAction: TCloseAction);
begin

end;

initialization
  {$I intdialog.lrs}

end.

