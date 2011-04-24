unit playerframe;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, ExtCtrls, EditBtn, StdCtrls;

type

  { TfPlayer }

  TfPlayer = class(TFrame)
    Bevel1: TBevel;
    cbPlayer: TCheckBox;
    fneAI: TFileNameEdit;
    leInfluence: TLabeledEdit;
    procedure cbPlayerChange(Sender: TObject);
  private
    { private declarations }
  public
    { public declarations }
  end; 

implementation

{ TfPlayer }

procedure TfPlayer.cbPlayerChange(Sender: TObject);
begin
  fneAI.Enabled := not cbPlayer.Checked;
end;

initialization
  {$I playerframe.lrs}

end.

