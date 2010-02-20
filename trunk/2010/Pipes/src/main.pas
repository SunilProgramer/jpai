unit main;

interface

uses
  Windows, Messages, SysUtils, Variants, Classes, Graphics, Controls, Forms,
  Dialogs, ExtCtrls, StdCtrls, Spin, ComCtrls, math;

const
  kindcount: integer = 12;
  kinds: array[0..11] of Integer = (0, 3, 5, 6, 7, 9, 10, 11, 12, 13, 14, 15);
  colors: array[0..6] of TColor = (clBlack, clRed, clBlue, clYellow, clPurple, clMoneyGreen, clGreen);
  //cellsize: integer = 64;
  //pipesize: integer = 8;
type
  TfrmMain = class(TForm)
    pTools: TPanel;
    pbMain: TPaintBox;
    seW: TSpinEdit;
    seH: TSpinEdit;
    sePC: TSpinEdit;
    brnStart: TButton;
    lbScores: TListBox;
    cbEdit: TCheckBox;
    tbCellsize: TTrackBar;
    lTurn: TLabel;
    procedure FormCreate(Sender: TObject);
    procedure pbMainPaint(Sender: TObject);
    procedure brnStartClick(Sender: TObject);
    procedure pbMainMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure pbMainMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
    procedure tbCellsizeChange(Sender: TObject);
    procedure pbMainMouseDown(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
  private
    { Private declarations }
  public
    xs: Integer;
    ys: Integer;
    pc: Integer;
    turn: Integer;
    field: array[0..100, 0..100] of Integer;
    affection: array[0..100, 0..100] of Integer;
    base: array[0..100, 0..100] of Integer;
    rotation: array[0..100, 0..100] of Integer;
    visited: array[0..100, 0..100] of Boolean;
    scores: array[0..100] of Integer;
    mx, my: Integer;
    stx, sty, sto: Integer;
    pr: boolean;

    procedure PaintRegion(x, y: Integer);
    procedure RePaintRegion(x, y: Integer);
    procedure Check(x, y, z: Integer);
    procedure Count();
  end;

var
  frmMain: TfrmMain;

implementation

{$R *.dfm}

procedure TfrmMain.brnStartClick(Sender: TObject);
var
  x: Integer;
  y: Integer;
begin
  xs := seW.Value;
  ys := seH.Value;
  pc := sePC.Value;
  turn := 0;
  lbScores.Items.Clear;
  for x := 0 to pc - 1 do
  begin
    scores[x] := 0;
    lbScores.Items.Add('Player'+inttostr(x+1));
  end;
  for x := 0 to xs - 1 do
    for y := 0 to ys - 1 do
    begin
      rotation[x,y] := random(3);
      field[x,y] := kinds[random(kindcount)];
      affection[x,y] := 0;
      base[x,y] := -1;
    end;
    lTurn.Caption := 'Player'+inttostr(turn+1) + 'turn';

  sto := -1;
  pbMain.Repaint;
end;

procedure TfrmMain.Check(x, y, z: Integer);
begin
  if (((x < 0) or (y < 0)) or ((x >= xs) or (y >= ys))) or (field[x,y] and z = 0) then
    exit;
  if (base[x,y] <> turn) and (base[x,y] <> -1) then
    exit;
  if visited[x,y] then
    exit;
  //inc(scores[turn]);
  visited[x,y] := true;
  affection[x,y] := turn + 1;
  RePaintRegion(x, y);
  if field[x,y] and 1 <> 0 then
    Check(x+1,y,4);
  if field[x,y] and 2 <> 0 then
    Check(x,y-1,8);
  if field[x,y] and 4 <> 0 then
    Check(x-1,y,1);
  if field[x,y] and 8 <> 0 then
    Check(x,y+1,2);
end;

procedure TfrmMain.Count;
var
  x: Integer;
  y: Integer;
  I: Integer;
begin
  for x := 0 to xs - 1 do
    for y := 0 to ys - 1 do
    begin
      if affection[x,y] <> 0 then
      begin
        for I := 0 to 3 do
        if (field[x,y] and Round(power(2, i)) <> 0) then
         inc(scores[affection[x,y] - 1]);
      end;
    end;
end;

procedure TfrmMain.FormCreate(Sender: TObject);
var
  x: Integer;
  y: Integer;
begin
  xs := 10;
  randomize;
  ys := 10;
  pc := 2;
  lbScores.Items.Clear;
  for x := 0 to pc - 1 do
  begin
    scores[x] := 0;
    lbScores.Items.Add('Player'+inttostr(x+1));
  end;
  for x := 0 to xs - 1 do
    for y := 0 to ys - 1 do
    begin
      rotation[x,y] := (random(3));
      field[x,y] := kinds[random(kindcount)];
      affection[x,y] := 0;
      base[x,y] := -1;
    end;
  sto := -1;
  pr := false;
end;

procedure TfrmMain.PaintRegion(x, y: Integer);
var
  xo, yo: Integer;
  rot: string;
  iRect: TRect;
  cl, cl1: TColor;
  cellsize, pipesize: integer;
begin
  cellsize := tbCellsize.Position;
  pipesize := Round(sqrt(cellsize));
  xo := x*cellsize;
  yo := y*cellsize;
  cl := pbMain.Canvas.Brush.Color;
  cl1 := pbMain.Canvas.Pen.Color;
  pbMain.Canvas.Brush.Color := clWhite;
  pbMain.Canvas.Pen.Color := clWhite;
  pbMain.Canvas.Pen.Style := psSolid;
  if base[x,y] <> -1 then
  begin
    pbMain.Canvas.Brush.Color := clSilver;
    pbMain.Canvas.Pen.Color := clBlack;
  end;
  if (stx =x) and (sty = y) and (sto <> -1) then
  begin
    pbMain.Canvas.Brush.Color := RGB(128,255,128);
    pbMain.Canvas.Pen.Color := RGB(64, 128, 64);
    pbMain.Canvas.Pen.Style := psDot;
  end;
  pbMain.Canvas.FillRect(Rect(xo, yo, xo+cellsize,yo+cellsize));
  if pbMain.Canvas.Pen.Color <> clWhite then
    pbMain.Canvas.Rectangle(xo, yo, xo+cellsize,yo+cellsize);
  pbMain.Canvas.Brush.Color := cl;
  if (field[x,y] and 1) <> 0 then
  begin
    iRect.Left := xo + (cellsize div 2);
    iRect.Top := yo - (pipesize div 2) + (cellsize div 2);
    iRect.Right := xo + cellsize;
    iRect.Bottom := iRect.Top + pipesize;
    pbMain.Canvas.FillRect(iRect);
  end;
  if (field[x,y] and 2) <> 0 then
  begin
    iRect.Left := xo - (pipesize div 2) + (cellsize div 2);;
    iRect.Top := yo;
    iRect.Right := iRect.Left + pipesize;
    iRect.Bottom := yo + (cellsize div 2);
    pbMain.Canvas.FillRect(iRect);
  end;
  if (field[x,y] and 4) <> 0 then
  begin
    iRect.Left := xo;
    iRect.Top := yo - (pipesize div 2) + (cellsize div 2);
    iRect.Right := xo + (cellsize div 2);
    iRect.Bottom := iRect.Top + pipesize;
    pbMain.Canvas.FillRect(iRect);
  end;
  if (field[x,y] and 8) <> 0 then
  begin
    iRect.Left := xo - (pipesize div 2) + (cellsize div 2);;
    iRect.Top := yo + (cellsize div 2);
    iRect.Right := iRect.Left + pipesize;
    iRect.Bottom := yo + cellsize;
    pbMain.Canvas.FillRect(iRect);
  end;
  if rotation[x, y] = 0 then
    rot := 'ccw'
  else if rotation[x, y] = 1 then
    rot := 'cw'
  else
    rot := 'no';
  (*cl := pbMain.Canvas.Brush.Color;
  pbMain.Canvas.Brush.Color := clWhite;
  pbMain.Canvas.TextOut(xo, yo, rot);
  pbMain.Canvas.Brush.Color := cl;   *)
end;

procedure TfrmMain.pbMainMouseDown(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  old: Integer;
  i: Integer;
  j: Integer;
begin
  x := x div tbCellsize.Position;
  y := y div tbCellsize.Position;
//  pbMain.Canvas.Brush.Color := clWhite;
//  PaintRegion(x, y);
  if cbEdit.Checked then
  begin
    if Button = mbRight then
    begin
      i := -1;
      pr := true;
    field[x,y] := kinds[(i+1) mod kindcount];
//    pbMain.Canvas.Brush.Color := affection[x,y];
    RePaintRegion(x, y);
    end;

  end;
end;

procedure TfrmMain.pbMainMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
var
  i: Integer;
begin
  x := x div tbCellsize.Position;
  y := y div tbCellsize.Position;
  if not pr then
    exit;
  if cbEdit.Checked then
  begin
    i := -1;
    if field[x,y] = 0 then
      exit;

    field[x,y] := 0;
//    pbMain.Canvas.Brush.Color := affection[x,y];
    RePaintRegion(x, y);
    exit;
  end;

end;

procedure TfrmMain.pbMainMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
  old: Integer;
  i: Integer;
  j: Integer;
begin
  pr := false;
  x := x div tbCellsize.Position;
  y := y div tbCellsize.Position;
//  pbMain.Canvas.Brush.Color := clWhite;
//  PaintRegion(x, y);
  if cbEdit.Checked then
  begin
    if Button = mbLeft then
    begin
      for i := 0 to kindcount - 1 do
        if field[x,y] = kinds[i] then
          break;
    end
    else
      i := -1;

    field[x,y] := kinds[(i+1) mod kindcount];
//    pbMain.Canvas.Brush.Color := affection[x,y];
    RePaintRegion(x, y);
    exit;
  end;

  //rotating
  //pbMain.Canvas.Brush.Color := clWhite;
  //PaintRegion(x, y);
//  if Button = mbLeft then
  //if Rotation[x,y] = 0 then
  if (Button = mbLeft) and (base[x,y] = -1) then
  begin
    if (sto <> -1) and ((stx=x) and (sty=y)) then
    begin
      field[x,y] := (field[x,y] shl 1) and 15 + Integer((field[x,y] shl 1) and 16 = 16);
      RepaintRegion(x, y);
    end;
    if (sto = -1) and not (base[x, y] <> -1) then
    begin
      sto := field[x,y];
      stx := x;
      sty := y;
      RepaintRegion(stx, sty);
    end;
    if (stx <> x) or (sty <> y) and not (base[x, y] <> -1) then
    begin
      field[stx, sty] := sto;
      sto := field[x, y];
      i := stx;
      j := sty;
      stx := x;
      sty := y;
      RePaintRegion(i, j);
      RePaintRegion(stx, sty);
    end;
//    PaintRegion(x, y);
    exit;
  end;
  if sto = -1 then
    exit;
  if Button = mbRight then
  begin
    if (sto <> -1) and ((stx=x) and (sty=y)) then
    begin
      base[stx, sty] := turn;
    end;
    sto := -1;
  end;
  //if Rotation[x,y] = 1 then
//    field[x,y] := field[x,y] shr 1 + (field[x,y] and 1) shl 3;
  for i := 0 to xs - 1 do
    for j := 0 to ys - 1 do
      visited[i,j] := false;
  Check(x, y, 15);
  Count();
  for i := 0 to pc - 1 do
    lbScores.Items.Strings[i] := ('Player'+inttostr(i+1) + ': ' + inttostr(scores[i]));


//  affection[x,y] := turn + 1;
  turn := (turn + 1) mod pc;
  lTurn.Caption := 'Player'+inttostr(turn+1) + ' turn';
  //pbMain.Canvas.Brush.Color := colors[affection[x,y]];
//  PaintRegion(x, y);
end;

procedure TfrmMain.pbMainPaint(Sender: TObject);
var
  x: Integer;
  y: Integer;
begin
  pbMain.Canvas.Brush.Color := clWhite;
  pbMain.Canvas.FillRect(Rect(0, 0, pbMain.Width, pbMain.Height));
  for x := 0 to xs - 1 do
  begin
    for y := 0 to ys - 1 do
    begin
      pbMain.Canvas.Brush.Color := colors[affection[x,y]];
      PaintRegion(x, y);
    end;
  end;
end;

procedure TfrmMain.RePaintRegion(x, y: Integer);
begin
  pbMain.Canvas.Brush.Color := clWhite;
  PaintRegion(x, y);
  pbMain.Canvas.Brush.Color := colors[affection[x,y]];
  PaintRegion(x, y);
end;

procedure TfrmMain.tbCellsizeChange(Sender: TObject);
begin
  pbMain.Repaint;
end;

end.
