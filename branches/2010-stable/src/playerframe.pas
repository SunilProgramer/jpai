unit playerframe;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, ExtCtrls, EditBtn;

type

  { TfPlayer }

  TfPlayer = class(TFrame)
    Bevel1: TBevel;
    fneAI: TFileNameEdit;
    leInfluence: TLabeledEdit;
    leScores: TLabeledEdit;
  private
    { private declarations }
  public
    { public declarations }
  end; 

implementation

initialization
  {$I playerframe.lrs}

end.

