unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, process, FileUtil, LResources, Forms, Controls, Graphics,
  Dialogs, ComCtrls, Buttons, EditBtn, StdCtrls, ExtCtrls, Spin, Math,
  playerframe, map {$IFDEF UNIX}, BaseUnix{$ENDIF};

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
    prAI: TProcess;
    sbStep: TSpeedButton;
    sbReset: TSpeedButton;
    sbAddPlayer: TSpeedButton;
    sbRemPlayer: TSpeedButton;
    sbHorizontal: TScrollBar;
    sbVertical: TScrollBar;
    seStepsCount: TSpinEdit;
    sbAccept: TSpeedButton;
    sbExpand: TSpeedButton;
    tbMain: TToolBar;
    tbScale: TTrackBar;
    tbStart: TToggleBox;
    procedure fneMapAcceptFileName(Sender: TObject; var Value: String);
    procedure FormCreate(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure pbDrawAreaPaint(Sender: TObject);
    procedure pbDrawAreaResize(Sender: TObject);
    procedure sbAcceptClick(Sender: TObject);
    procedure sbRemPlayerClick(Sender: TObject);
    procedure sbAddPlayerClick(Sender: TObject);
    procedure sbHorizontalChange(Sender: TObject);
    procedure sbResetClick(Sender: TObject);
    procedure sbStartClick(Sender: TObject);
    procedure sbStepClick(Sender: TObject);
    procedure sbExpandClick(Sender: TObject);
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
     StepsLeft: Integer;
     procedure AddPlayer(const AI: String);
     function PlayersCount(): Integer;
     procedure RemovePlayer;
     procedure Draw;
     procedure DrawSegment(x, y: Integer; C: TCanvas);
     procedure RefreshScores();
     procedure Disable();
     procedure Enable();
     procedure ShowFinalMessage();
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
    sbExpand.Click();
    sbReset.Click();
  end;
  pbDrawAreaResize(self);
end;


procedure TfrmMain.FormCreate(Sender: TObject);
begin
  m := TMap.Create();
  AppPath := ExtractFileDir(Application.ExeName);
  FRunning := false;

  fneMap.InitialDir := AppPath + DirectorySeparator + 'maps';
  fneMap.FileName := fneMap.InitialDir + DirectorySeparator + StartMap + '.map';

  AddPlayer(DefaultAI);
  AddPlayer(DefaultAI);
  fneMapAcceptFileName(nil, fneMap.FileName);
  inc(tbMain.Height, 7);
  sbAccept.Click();
  prAI.CommandLine := AppPath + DirectorySeparator + 'runarea'+ DirectorySeparator + 'ai';
  prAI.CurrentDirectory := AppPath + DirectorySeparator + 'runarea';
end;

procedure TfrmMain.FormResize(Sender: TObject);
begin
  tbMain.Width := frmMain.ClientWidth;
  pDrawArea.Width := frmMain.ClientWidth - gbPlayers.Width;
  Application.ProcessMessages;
end;


procedure TfrmMain.pbDrawAreaPaint(Sender: TObject);
begin
  Draw;
end;

procedure TfrmMain.pbDrawAreaResize(Sender: TObject);
begin
  sbHorizontal.Max := max(0, m.Width*tbScale.Position - pbDrawArea.Width);
  sbVertical.Max := max(0, m.Height*tbScale.Position - pbDrawArea.Height);
//  showmessage(inttostr(sbHorizontal.Max));
//  sbHorizontal.PageSize := max(2(*sbHorizontal.Max div 10*), 1);//pbDrawArea.Width;
//  sbVertical.PageSize := pbDrawArea.Height;
end;

procedure TfrmMain.sbAcceptClick(Sender: TObject);
begin
  if TSpeedButton(Sender).Caption = 'Ok' then
  begin
    seStepsCount.Visible := false;
    TSpeedButton(Sender).Caption := '>>';
    StepsLeft := seStepsCount.Value;
    sbReset.Click();
  end
  else
  begin
    seStepsCount.Visible := true;
    TSpeedButton(Sender).Caption := 'Ok';
  end;
end;

procedure TfrmMain.sbRemPlayerClick(Sender: TObject);
begin
  if PlayersCount() > 2 then
  begin
    RemovePlayer;
    sbResetClick(nil);
  end;
end;

procedure TfrmMain.sbAddPlayerClick(Sender: TObject);
begin
  if PlayersCount() < 10 then
  begin
    AddPlayer(DefaultAI);
    sbResetClick(nil);
  end;
end;

procedure TfrmMain.sbHorizontalChange(Sender: TObject);
begin

end;

procedure TfrmMain.sbResetClick(Sender: TObject);
begin
  m.ResetValues();
  m.StepsLeft := StepsLeft;
  DeleteDirectory(AppPath + DirectorySeparator + 'temp', true);
  RefreshScores();
  Draw;
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
    CopyFile(AAI, dir + DirectorySeparator + 'ai'+GetExeExt);
    {$IFDEF UNIX}
    FpChmod(dir + DirectorySeparator + 'ai', &775);
    {$ENDIF}
    CheckIfFileIsExecutable(dir + DirectorySeparator + 'ai'+GetExeExt);
    if FileExists(AppPath + DirectorySeparator + 'temp' + DirectorySeparator +
      'temp' + IntToStr(APlayer)) then
      CopyFile(AppPath + DirectorySeparator + 'temp' + DirectorySeparator + 'temp' +
        IntToStr(APlayer)+'.txt', dir + DirectorySeparator + 'temp.txt');
    m.Save(dir + DirectorySeparator + 'input.txt');//mb here place an argument for player num
    prAI.Execute();
    m.ProcessAIOutput(dir + DirectorySeparator + 'output.txt', APlayer);
    if FileExists(dir + DirectorySeparator + 'temp.txt') then
      CopyFile(dir + DirectorySeparator + 'temp.txt', AppPath + DirectorySeparator + 'temp' +
        DirectorySeparator + 'temp' + IntToStr(APlayer)+'.txt');
  end;

begin
  if (m.StepsLeft = 0) or m.Full then
  begin
    ShowFinalMessage();
    FRunning := false;
    Enable();
    exit;
  end;
  if FRunning and (Sender <> nil) then exit;
  dir := AppPath + DirectorySeparator + 'runarea';
  ForceDirectories(AppPath + DirectorySeparator + 'temp');
  for i := 0 to PlayersCount() - 1 do
    if not FileExists(players[i].frm.fneAI.FileName) then
    begin
      showmessage('Пожалуйста выберите файл искусственного интеллекта для Игрока ' +
        IntToStr(i+1) + '.');
      exit;
    end;
  for i := 0 to PlayersCount() - 1 do
    RunAI(players[i].frm.fneAI.FileName, i);
  inc(m.StepsPassed);
  dec(m.StepsLeft);
  RefreshScores();
  Draw;
end;

procedure TfrmMain.sbExpandClick(Sender: TObject);
begin
  tbScale.Position := min(floor(pbDrawArea.Width/m.Width), floor(pbDrawArea.Height/m.Height));
  tbScaleChange(tbScale);
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
  btm.Canvas.Brush.Color := clBlack;
  btm.Canvas.FillRect(0, 0, pbDrawArea.Width, pbDrawArea.Height);

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
  pbDrawArea.Canvas.CopyRect(Rect(0, 0, pbDrawArea.Width, pbDrawArea.Height),
    btm.Canvas, Rect(0, 0, pbDrawArea.Width, pbDrawArea.Height));
  btm.Free();
  Application.ProcessMessages;
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
  if C.Pen.Color = clBlack then
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
  sbStep.Caption := 'Ход ' + IntToStr(m.StepsPassed);
  Caption := 'Pipe Control "'+ExtractFileName(fneMap.FileName)+'" '+
    IntToStr(m.Width) + 'x' + IntToStr(m.Height) + ', ' + IntToStr(PlayersCount()) +
    ' players. Step ' + IntToStr(m.StepsPassed) + '/' + IntToStr(StepsLeft);
end;

procedure TfrmMain.Disable();
begin
  fneMap.Enabled := false;
  sbStep.Enabled := false;
  sbReset.Enabled := false;
  sbAddPlayer.Enabled := false;
  sbRemPlayer.Enabled := false;
  seStepsCount.Enabled := false;
end;

procedure TfrmMain.Enable();
begin
  fneMap.Enabled := true;
  sbStep.Enabled := true;
  sbReset.Enabled := true;
  sbAddPlayer.Enabled := true;
  sbRemPlayer.Enabled := true;
  seStepsCount.Enabled := true;
end;

procedure TfrmMain.ShowFinalMessage();
var
  i: Integer;
  maxs1, maxs2: Integer;
  s1, s2: String;
begin
  maxs1 := 0;
  maxs2 := 0;
  s1 := '';
  s2 := '';
  for i := 1 to PlayersCount() do
  begin
    maxs1 := max(maxs1, m.Scores1[i]);
    maxs2 := max(maxs2, m.Scores2[i]);
  end;
  for i := 1 to PlayersCount() do
  begin
    if m.Scores1[i] = maxs1 then
      s1 := s1 + ' Player' + IntToStr(i) + ';';
    if m.Scores2[i] = maxs2 then
      s2 := s2 + ' Player' + IntToStr(i) + ';';
  end;
  ShowMessage('1. Очки: '+IntToStr(maxs1) + ',' + s1 + #10 + '2. Захвачено территории: ' +
    IntToStr(maxs2) + ',' + s2);
end;



procedure TfrmMain.loadAI(Sender: TObject; var Value: String);
begin
  if FRunning then
  begin
    Value := '';
    exit;
  end;
  sbResetClick(nil);
end;

procedure TfrmMain.tbStartChange(Sender: TObject);
begin
  FRunning := TToggleBox(Sender).checked;
  while FRunning and not m.GameOver do
    begin
      Disable();
      try
      sbStepClick(nil);
      except
      end;
    end;
  Enable();
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
  frm.fneAI.InitialDir := ExtractFileDir(Application.ExeName) + DirectorySeparator + 'ai';
  frm.fneAI.FileName := frm.fneAI.InitialDir + DirectorySeparator + AI + GetExeExt;
end;

destructor TPlayer.Destroy();
begin
  frm.Destroy();
end;

initialization
  {$I main.lrs}

end.

