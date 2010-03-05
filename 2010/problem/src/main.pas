unit main;

{$mode objfpc}{$H+}
interface

uses
  Classes, SysUtils, FileUtil, LResources, Forms, Controls, Graphics, Dialogs, Variants,
  ExtCtrls, StdCtrls, ComCtrls, Buttons, EditBtn, map, PlayerFrame, math;

const
  StartMap = 'test';
  DefaultAI = 'random';
  PLAYER_COLORS: array[0..9] of TColor = (clBlack, clRed, clBlue, clGreen, clYellow, clMaroon, clLime, clAqua, clPurple, clTeal);

type

  { TPlayer }

  TPlayer = class
    private
      frm: TfPlayer;
    public
      constructor Create(n: Integer; AI: String; Parent: TWinControl);
      destructor Destroy();
  end;

  { TfrmMain }

  TfrmMain = class(TForm)
    fneMap: TFileNameEdit;
    gbPlayers: TGroupBox;
    pbDrawArea: TPaintBox;
    pDrawArea: TPanel;
    sbStep: TSpeedButton;
    sbStart: TSpeedButton;
    sbReset: TSpeedButton;
    sbAddPlayer: TSpeedButton;
    RemPlayer: TSpeedButton;
    sbHorizontal: TScrollBar;
    sbVertical: TScrollBar;
    tbMain: TToolBar;
    tbScale: TTrackBar;
    procedure fneMapAcceptFileName(Sender: TObject; var Value: String);
    procedure FormCreate(Sender: TObject);
    procedure pbDrawAreaPaint(Sender: TObject);
    procedure pbDrawAreaResize(Sender: TObject);
    procedure RemPlayerClick(Sender: TObject);
    procedure sbAddPlayerClick(Sender: TObject);
    procedure tbScaleChange(Sender: TObject);
   private
    { Private declarations }
   public
     m: TMap;
     AppPath: String;
     players: array of TPlayer;
     procedure AddPlayer(const AI: String);
     procedure RemovePlayer;
     procedure Draw;
     procedure DrawSegment(x, y: Integer; C: TCanvas);
  end;

var
  frmMain: TfrmMain;

implementation



{ TfrmMain }


procedure TfrmMain.fneMapAcceptFileName(Sender: TObject; var Value: String
  );
begin
  try
    m.Load(Value);
  except
    //
  end;
  DeleteDirectory(AppPath + '\temp', true);
//  RefreshScores;
end;

procedure TfrmMain.FormCreate(Sender: TObject);
begin
  m := TMap.Create();
  AppPath := ExtractFileDir(Application.ExeName);
  fneMap.InitialDir := AppPath + '\maps';
  fneMap.FileName := fneMap.InitialDir + '\' + StartMap + '.map';
  fneMapAcceptFileName(self, fneMap.FileName);
  AddPlayer(DefaultAI);
  AddPlayer(DefaultAI);
end;

procedure TfrmMain.pbDrawAreaPaint(Sender: TObject);
begin
  Draw;
end;

procedure TfrmMain.pbDrawAreaResize(Sender: TObject);
begin
  sbHorizontal.Max := max(0, m.Width*tbScale.Position - pDrawArea.Width);
  sbVertical.Max := max(0, m.Height*tbScale.Position - pDrawArea.Height);
end;

procedure TfrmMain.RemPlayerClick(Sender: TObject);
begin
  RemovePlayer;
end;

procedure TfrmMain.sbAddPlayerClick(Sender: TObject);
begin
  AddPlayer(DefaultAI);
end;

procedure TfrmMain.tbScaleChange(Sender: TObject);
begin
  sbHorizontal.Max := max(0, m.Width*tbScale.Position - pDrawArea.Width);
  sbVertical.Max := max(0, m.Height*tbScale.Position - pDrawArea.Height);
  Draw;
end;


procedure TfrmMain.AddPlayer(const AI: String);
var
  frm: TPlayer;
  n: Integer;
begin
  n := Length(players) + 1;
  try
    frm := TPlayer.Create(n, AI, gbPlayers);
  except
    frm.Free;
    raise;
  end;
  SetLength(players, n);
  players[High(players)] := frm;
end;

procedure TfrmMain.RemovePlayer;
var
  frm: TPlayer;
  n: Integer;
begin
  if Length(players) <= 2 then
    exit;
  players[High(players)].Destroy;
  SetLength(players, Length(players) - 1);
end;

procedure TfrmMain.Draw;
var
  lx, ly, ox, oy, w, h, i, j: Integer;
  btm: TBitmap;
begin
  lx := -sbHorizontal.Position;
  ly := -sbVertical.Position;
  btm := TBitmap.Create();
  btm.SetSize(pbDrawArea.Width, pbDrawArea.Height);
//  showmessage(IntToStr(Integer(btm.PixelFormat)));
//  btm.PixelFormat:=

  ox := floor(( -lx ) / tbScale.Position);
  oy := floor(( -ly ) / tbScale.Position);

  w := min(ceil(pbDrawArea.Width / tbScale.Position) + 1, m.Width);
  h := min(ceil(pbDrawArea.Height / tbScale.Position) + 1, m.Height);

  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      btm.Canvas.Brush.Color := PLAYER_COLORS[m.Color[ox+i, oy+j]];
      DrawSegment(ox+i, oy+j, btm.Canvas);
    end;
  pbDrawArea.Canvas.CopyRect(btm.Canvas.ClipRect, btm.Canvas, btm.Canvas.ClipRect);
  btm.Free();
end;

procedure TfrmMain.DrawSegment(x, y: Integer; C: TCanvas);
var
  xo, yo: Integer;
  rot: string;
  iRect: TRect;
  cl, cl1: TColor;
  cellsize, pipesize: integer;
begin
  if (x >= m.Width) or (y >= m.Height) then
    exit;
  xo := x*tbScale.Position - sbHorizontal.Position;
  yo := y*tbScale.Position - sbVertical.Position;
  cellsize := tbScale.Position;
  pipesize := Round(sqrt(cellsize));
  cl := C.Brush.Color;
  cl1 := C.Pen.Color;
  C.Brush.Color := clWhite;
  C.Pen.Color := clWhite;
  C.Pen.Style := psSolid;
  if m.Bases[x,y] then
  begin
    C.Brush.Color := clSilver;
    C.Pen.Color := clBlack;
  end;
  C.FillRect(Rect(xo, yo, xo+cellsize,yo+cellsize));
  if C.Pen.Color <> clWhite then
    C.Rectangle(xo, yo, xo+cellsize,yo+cellsize);
  C.Brush.Color := cl;
  C.Pen.Color:= cl1;
  if (m.Field[x,y] and 2) <> 0 then
  begin
    iRect.Left := xo + (cellsize div 2);
    iRect.Top := yo - (pipesize div 2) + (cellsize div 2);
    iRect.Right := xo + cellsize;
    iRect.Bottom := iRect.Top + pipesize;
    C.FillRect(iRect);
  end;
  if (m.Field[x,y] and 4) <> 0 then
  begin
    iRect.Left := xo - (pipesize div 2) + (cellsize div 2);;
    iRect.Top := yo;
    iRect.Right := iRect.Left + pipesize;
    iRect.Bottom := yo + (cellsize div 2);
    C.FillRect(iRect);
  end;
  if (m.Field[x,y] and 8) <> 0 then
  begin
    iRect.Left := xo;
    iRect.Top := yo - (pipesize div 2) + (cellsize div 2);
    iRect.Right := xo + (cellsize div 2);
    iRect.Bottom := iRect.Top + pipesize;
    C.FillRect(iRect);
  end;
  if (m.Field[x,y] and 0) <> 0 then
  begin
    iRect.Left := xo - (pipesize div 2) + (cellsize div 2);;
    iRect.Top := yo + (cellsize div 2);
    iRect.Right := iRect.Left + pipesize;
    iRect.Bottom := yo + cellsize;
    C.FillRect(iRect);
  end;
end;


{ TPlayer }

constructor TPlayer.Create(n: Integer; AI: String; Parent: TWinControl);
begin
  frm := TfPlayer.Create(Parent);
  frm.Parent := Parent;
  frm.Align := alTop;
  frm.Top := MaxInt;
  frm.Name := frm.Name + IntToStr(n);
  frm.fneAI.Font.Color := PLAYER_COLORS[n];
  frm.fneAI.InitialDir := ExtractFileDir(Application.ExeName) + '\ai';
  frm.fneAI.FileName := frm.fneAI.InitialDir + '\' + AI + '.exe';
end;

destructor TPlayer.Destroy();
begin
  frm.Destroy();
end;

initialization
  {$I main.lrs}


end.
