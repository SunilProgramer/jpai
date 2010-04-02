unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, process, FileUtil, LResources, Forms, Controls, Graphics,
  Dialogs, ComCtrls, Buttons, EditBtn, StdCtrls, ExtCtrls, Math, playerframe, map;

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
    fneTmp: TFileNameEdit;
    gbPlayers: TGroupBox;
    pbDrawArea: TPaintBox;
    pDrawArea: TPanel;
    Process1: TProcess;
    sbStep: TSpeedButton;
    sbReset: TSpeedButton;
    sbAddPlayer: TSpeedButton;
    RemPlayer: TSpeedButton;
    sbHorizontal: TScrollBar;
    sbVertical: TScrollBar;
    tbMain: TToolBar;
    tbScale: TTrackBar;
    tbStart: TToggleBox;
    procedure fneMapAcceptFileName(Sender: TObject; var Value: String);
    procedure FormCreate(Sender: TObject);
    procedure pbDrawAreaPaint(Sender: TObject);
    procedure pbDrawAreaResize(Sender: TObject);
    procedure RemPlayerClick(Sender: TObject);
    procedure sbAddPlayerClick(Sender: TObject);
    procedure sbHorizontalChange(Sender: TObject);
    procedure sbResetClick(Sender: TObject);
    procedure sbStartClick(Sender: TObject);
    procedure sbStepClick(Sender: TObject);
    procedure tbScaleChange(Sender: TObject);
    procedure loadAI(Sender: TObject; var Value: String);
    procedure tbStartChange(Sender: TObject);
   private
    { Private declarations }
   public
     m: TMap;
     AppPath: String;
     players: array of TPlayer;
     FRunning: Boolean;
     procedure AddPlayer(const AI: String);
     function PlayersCount(): Integer;
     procedure RemovePlayer;
     procedure Draw;
     procedure DrawSegment(x, y: Integer; C: TCanvas);
     procedure RefreshScores();
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
  finally
    Draw;
    //
  end;
  DeleteDirectory(AppPath + '\temp', true);
//  RefreshScores;
end;

procedure TfrmMain.FormCreate(Sender: TObject);
begin
  m := TMap.Create();
  AppPath := ExtractFileDir(Application.ExeName);
  FRunning := false;
  fneMap.InitialDir := AppPath + '\maps';
  fneMap.FileName := fneMap.InitialDir + '\' + StartMap + '.map';
//  fneMapAcceptFileName(self, fneMap.);
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
  sbResetClick(nil);
end;

procedure TfrmMain.sbAddPlayerClick(Sender: TObject);
begin
  AddPlayer(DefaultAI);
  sbResetClick(nil);
end;

procedure TfrmMain.sbHorizontalChange(Sender: TObject);
begin

end;

procedure TfrmMain.sbResetClick(Sender: TObject);
begin
  m.ResetValues();
  Draw;
  RefreshScores();
end;

procedure TfrmMain.sbStartClick(Sender: TObject);
begin

end;

procedure TfrmMain.sbStepClick(Sender: TObject);
var
  dir: String;
  i: Integer;

  procedure RunAI(const AAI: String; APlayer: Integer);
  begin
    DeleteDirectory(dir, true);
    CopyFile(AAI, dir + '/ai.exe');
//    CopyFile(AppPath + '/temp/temp' + IntToStr(APlayer), dir + '/temp.txt');
    m.Save(dir + '/input.txt');//mb here place an argument for player num
    Process1.Active := true;
    m.ProcessAIOutput(dir + '/output.txt', APlayer);
    //CopyFile(dir + '/temp.txt', AppPath + '/temp/temp' + IntToStr(APlayer));
  end;

begin
  if FRunning and (Sender <> nil) then exit;
  dir := AppPath + '/runarea';
  ForceDirectories(AppPath + '/temp');
  for i := 0 to PlayersCount - 1 do
    RunAI(players[i].frm.fneAI.FileName, i);
//  FMap.NextStep;
//  RefreshScores;
  Draw;
  RefreshScores();
  Application.ProcessMessages;
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
  if n >= 10 then
    exit;
  try
    frm := TPlayer.Create(n, AI, gbPlayers);
    frm.frm.fneAI.OnAcceptFileName := fneTmp.OnAcceptFileName;
  except
    frm.Free;
    raise;
  end;
  SetLength(players, n);
//  m.PlayersCount := n;
  players[High(players)] := frm;
  m.PlayersCount := PlayersCount();
end;

function TfrmMain.PlayersCount(): Integer;
begin
  Result := Length(players);
end;

procedure TfrmMain.RemovePlayer;
begin
  if Length(players) <= 2 then
    exit;
  players[High(players)].Destroy;
  SetLength(players, Length(players) - 1);
  m.PlayersCount := PlayersCount();
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
  if (m.Field[x,y] and 4) <> 0 then//right
  begin
    iRect.Left := xo + (cellsize div 2);
    iRect.Top := yo - (pipesize div 2) + (cellsize div 2);
    iRect.Right := xo + cellsize;
    iRect.Bottom := iRect.Top + pipesize;
    C.FillRect(iRect);
  end;
  if (m.Field[x,y] and 8) <> 0 then//up
  begin
    iRect.Left := xo - (pipesize div 2) + (cellsize div 2);
    iRect.Top := yo;
    iRect.Right := iRect.Left + pipesize;
    iRect.Bottom := yo + (cellsize div 2);
    C.FillRect(iRect);
  end;
  if (m.Field[x,y] and 1) <> 0 then//left
  begin
    iRect.Left := xo;
    iRect.Top := yo - (pipesize div 2) + (cellsize div 2);
    iRect.Right := xo + (cellsize div 2);
    iRect.Bottom := iRect.Top + pipesize;
    C.FillRect(iRect);
  end;
  if (m.Field[x,y] and 2) <> 0 then//down
  begin
    iRect.Left := xo - (pipesize div 2) + (cellsize div 2);
    iRect.Top := yo + (cellsize div 2);
    iRect.Right := iRect.Left + pipesize;
    iRect.Bottom := yo + cellsize;
    C.FillRect(iRect);
  end;
end;

procedure TfrmMain.RefreshScores();
var
   i: Integer;
begin
  m.CalculateScores();
  for i := 0 to PlayersCount() - 1 do
  begin
    players[i].frm.leInfluence.Text := inttostr(m.Scores2[i + 1]);
    players[i].frm.leScores.Text := inttostr(m.Scores1[i + 1]);
  end;
end;

procedure TfrmMain.loadAI(Sender: TObject; var Value: String);
begin
  sbResetClick(nil);
end;

procedure TfrmMain.tbStartChange(Sender: TObject);
begin
  FRunning := not FRunning;
  while FRunning and not m.GameOver do
    begin
      sbStepClick(nil);
    end;
  FRunning := false;
  tbStart.Checked := false;
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

