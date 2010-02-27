unit main;

{$mode objfpc}{$H+}
interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, Controls, Graphics, Dialogs, Messages,  Variants,
  ExtCtrls, StdCtrls, Spin, ComCtrls, math;

type

  { TfrmMain }

  TfrmMain = class(TForm)
    pnlTop: TPanel;
  private
    { Private declarations }
  public
  end;

var
  frmMain: TfrmMain;

implementation



initialization
  {$I main.lrs}


end.
