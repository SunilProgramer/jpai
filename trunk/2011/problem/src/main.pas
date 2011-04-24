unit main;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, process, FileUtil, LResources, Forms, Controls, Graphics,
  Dialogs, ComCtrls, Buttons, EditBtn, StdCtrls, ExtCtrls, Spin, AsyncProcess,
  Math, playerframe, map, aihandler, intdialog {$IFDEF UNIX}, BaseUnix{$ENDIF};

const
  StartMap = 'test';

type

  { TPipeAIHandler }

  TPipeAIHandler = class(TAIHandler)
  protected
    procedure FinishStep(); override;
    procedure Redraw(); override;
    procedure ShowFinalMessage(); override;
    procedure SetEnabled(enbl: Boolean); override;
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
    sbDebug: TStatusBar;
    tbMain: TToolBar;
    tbScale: TTrackBar;
    tbStart: TToggleBox;
    tTerminateTimer: TTimer;
    procedure AsyncProcess1Terminate(Sender: TObject);
    procedure fneMapAcceptFileName(Sender: TObject; var Value: String);
    procedure FormCloseQuery(Sender: TObject; var CanClose: boolean);
    procedure FormCreate(Sender: TObject);
    procedure FormResize(Sender: TObject);
    procedure pbDrawAreaMouseMove(Sender: TObject; Shift: TShiftState; X,
      Y: Integer);
    procedure pbDrawAreaMouseUp(Sender: TObject; Button: TMouseButton;
      Shift: TShiftState; X, Y: Integer);
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
    procedure tbMainClick(Sender: TObject);
    procedure tbScaleChange(Sender: TObject);
    procedure loadAI(Sender: TObject; var Value: String);
    procedure tbStartChange(Sender: TObject);
    procedure tTerminateTimerTimer(Sender: TObject);
   private
    { Private declarations }
   public
     Handler: TPipeAIHandler;
     executing: Boolean;
     procedure Draw;
     procedure DrawSegment(x, y: Integer; C: TCanvas);
     procedure RefreshScores();
     procedure Disable();
     procedure Enable();
     procedure ShowFinalMessage();
     function Blend(c1, c2: TColor; d: double): TColor;
  end;

var
  frmMain: TfrmMain;

implementation



{ TfrmMain }


procedure TfrmMain.fneMapAcceptFileName(Sender: TObject; var Value: String
  );
begin
  try
    Handler.map.Load(Value);
  finally
    sbExpand.Click();
    sbReset.Click();
  end;
  pbDrawAreaResize(self);
end;

procedure TfrmMain.AsyncProcess1Terminate(Sender: TObject);
begin
  showmessage('1');
end;

procedure TfrmMain.FormCloseQuery(Sender: TObject; var CanClose: boolean);
begin
  Handler.FRunning := false;
end;


procedure TfrmMain.FormCreate(Sender: TObject);
var
  str: String;
begin
  Handler := TPipeAIHandler.Create(gbPlayers);
  Handler.AcceptEvent := fneTmp.OnAcceptFileName;
  Handler.FRunning := false;

  fneMap.InitialDir := Handler.AppPath + DirectorySeparator + 'maps';
  fneMap.FileName := fneMap.InitialDir + DirectorySeparator + StartMap + '.map';
  str := fneMap.FileName;

  fneMapAcceptFileName(nil, str);
  tbMain.Height := tbMain.Height + 7;
  sbAccept.Click();
end;

procedure TfrmMain.FormResize(Sender: TObject);
begin
  tbMain.Width := frmMain.ClientWidth;
  pDrawArea.Width := frmMain.ClientWidth - gbPlayers.Width;
  Application.ProcessMessages;
end;

procedure TfrmMain.pbDrawAreaMouseMove(Sender: TObject; Shift: TShiftState; X,
  Y: Integer);
begin
  inc(X, sbHorizontal.Position);
  inc(Y, sbVertical.Position);
  X := X div tbScale.Position + 1;
  Y := Y div tbScale.Position + 1;
  sbDebug.Panels[0].Text := IntToStr(x) + ', ' + IntToStr(y) + ' ' + inttostr(sbHorizontal.Position);
end;

procedure TfrmMain.pbDrawAreaMouseUp(Sender: TObject; Button: TMouseButton;
  Shift: TShiftState; X, Y: Integer);
var
   rs: Integer;
begin
  if Handler.Manual then
  begin
    inc(X, sbHorizontal.Position);
    inc(Y, sbVertical.Position);
    Y := Y div tbScale.Position + 1;
    if y mod 2 = 0 then
       X := max(X - tbScale.Position div 2, 0);
    X := X div tbScale.Position + 1;
    rs := frmIntDialog.ShowModal;
    if rs = 1 then
    begin
      assignfile(output, Handler.RunDirectory + DirectorySeparator + 'output.txt');
      rewrite(output);
      writeln(x);
      writeln(y);
      writeln(frmIntDialog.seVal.Value);
      closefile(output);
      Handler.ManualEndAI;
    end;
  end;
end;


procedure TfrmMain.pbDrawAreaPaint(Sender: TObject);
begin
  Draw;
end;

procedure TfrmMain.pbDrawAreaResize(Sender: TObject);
begin
  sbHorizontal.Max := max(0, Handler.map.Width*tbScale.Position - pbDrawArea.Width);
  sbVertical.Max := max(0, Handler.map.Height*tbScale.Position - pbDrawArea.Height);
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
    Handler.StepsLeft := seStepsCount.Value;
  end
  else
  begin
    seStepsCount.Visible := true;
    TSpeedButton(Sender).Caption := 'Ok';
  end;
end;

procedure TfrmMain.sbRemPlayerClick(Sender: TObject);
begin
  if Handler.RemoveLastPlayer() then
    sbReset.Click;
end;

procedure TfrmMain.sbAddPlayerClick(Sender: TObject);
begin
  if Handler.AddPlayer(aihandler.DefaultAI) then
    sbReset.Click;
end;

procedure TfrmMain.sbHorizontalChange(Sender: TObject);
begin

end;

procedure TfrmMain.sbResetClick(Sender: TObject);
begin
  Handler.Refresh();
  RefreshScores();
  DeleteDirectory(Handler.AppPath + DirectorySeparator + 'temp', true);
  Draw;
end;

procedure TfrmMain.sbStartClick(Sender: TObject);
begin

end;

procedure TfrmMain.sbStepClick(Sender: TObject);
begin
  Disable();
  Handler.MakeStep();
end;

procedure TfrmMain.sbExpandClick(Sender: TObject);
begin
  tbScale.Position := min(floor(pbDrawArea.Width/Handler.map.Width), floor(pbDrawArea.Height/Handler.map.Height));
  tbScaleChange(tbScale);
end;

procedure TfrmMain.tbMainClick(Sender: TObject);
begin

end;

procedure TfrmMain.tbScaleChange(Sender: TObject);
begin
  sbHorizontal.Max := max(0, Handler.map.Width*tbScale.Position - pDrawArea.Width);
  sbVertical.Max := max(0, Handler.map.Height*tbScale.Position - pDrawArea.Height);
  Draw;
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
  btm.Canvas.Brush.Color := clWhite;
  btm.Canvas.FillRect(0, 0, pbDrawArea.Width, pbDrawArea.Height);

  ox := floor(( -lx ) / tbScale.Position);
  oy := floor(( -ly ) / tbScale.Position);

  w := min(ceil(pbDrawArea.Width / tbScale.Position) + 1, Handler.map.Width);
  h := min(ceil(pbDrawArea.Height / tbScale.Position) + 1, Handler.map.Height);

  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      btm.Canvas.Brush.Color := PLAYER_COLORS[Handler.map.Color[ox+i, oy+j]];
      if btm.Canvas.Brush.Color <> clWhite then
      begin
        btm.Canvas.Brush.Color := Blend(clWhite, btm.Canvas.Brush.Color, Handler.map.Influence[ox+i,oy+j]/Handler.map.MaxValue);
      end;
      DrawSegment(ox+i, oy+j, btm.Canvas);
    end;
  pbDrawArea.Canvas.CopyRect(Rect(0, 0, pbDrawArea.Width, pbDrawArea.Height),
    btm.Canvas, Rect(0, 0, pbDrawArea.Width, pbDrawArea.Height));
  btm.Free();
  Application.ProcessMessages;
end;

procedure TfrmMain.DrawSegment(x, y: Integer; C: TCanvas);
var
  xo, yo, x1, y1, i: Integer;
  iRect: TRect;
  cl, cl1: TColor;
  cellsize, pipesize, radius: integer;
  str: String;
begin
  if (x >= Handler.map.Width) or (y >= Handler.map.Height) then
    exit;
  cellsize := tbScale.Position;
  pipesize := Round(sqrt(cellsize));
  radius := round(cellsize/(2*sin(pi/6)));
  xo := x*tbScale.Position - sbHorizontal.Position + (cellsize div 2)*(y mod 2);
  yo := y*tbScale.Position - sbVertical.Position;// div 2;

  C.FillRect(Rect(xo, yo, xo+cellsize,yo+cellsize));

  Handler.map.Field[x,y];
  c.MoveTo(xo + cellsize div 2, yo);
  for i := 1 to 8 do
  begin
    x1 := round(xo + cellsize/2 + (radius/2)*cos(i*pi/3 + pi/6));
    y1 := round(yo +cellsize/2 - (radius/2)*sin(i*pi/3 + pi/6));
    c.LineTo(x1, y1);
    //c.MoveTo(x1, y1);
  end;
  str := IntToStr(Handler.Map.Influence[x, y]);
  c.Brush.Style := bsClear;
  c.Font.Height := round(radius * 0.8);
  c.TextOut(xo + (cellsize - c.TextWidth(str)) div 2,
  yo + (cellsize - c.TextHeight(str)) div 2, str);
  c.Brush.Style := bsSolid;
end;

procedure TfrmMain.RefreshScores();
begin
  sbStep.Caption := 'Ход ' + IntToStr(Handler.map.StepsPassed);
  Caption := 'Pipe Control "'+ExtractFileName(fneMap.FileName)+'" '+
    IntToStr(Handler.map.Width) + 'x' + IntToStr(Handler.map.Height) + ', ' + IntToStr(Handler.PlayersCount) +
    ' players. Step ' + IntToStr(Handler.map.StepsPassed) + '/' + IntToStr(Handler.StepsLeft);
end;

procedure TfrmMain.Disable();
begin
  fneMap.Enabled := false;
  sbStep.Enabled := false;
  sbReset.Enabled := false;
  sbAddPlayer.Enabled := false;
  sbRemPlayer.Enabled := false;
  seStepsCount.Enabled := false;
  sbAccept.Enabled := false;
end;

procedure TfrmMain.Enable();
begin
  fneMap.Enabled := true;
  sbStep.Enabled := true;
  sbReset.Enabled := true;
  sbAddPlayer.Enabled := true;
  sbRemPlayer.Enabled := true;
  seStepsCount.Enabled := true;
  sbAccept.Enabled := true;
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
  for i := 1 to Handler.PlayersCount do
  begin
    maxs1 := max(maxs1, Handler.map.Scores1[i]);
    maxs2 := max(maxs2, Handler.map.Scores2[i]);
  end;
  for i := 1 to Handler.PlayersCount do
  begin
    if Handler.map.Scores1[i] = maxs1 then
      s1 := s1 + ' Player' + IntToStr(i) + ';';
    if Handler.map.Scores2[i] = maxs2 then
      s2 := s2 + ' Player' + IntToStr(i) + ';';
  end;
  ShowMessage('1. Очки: '+IntToStr(maxs1) + ',' + s1 + #10 + '2. Захвачено территории: ' +
    IntToStr(maxs2) + ',' + s2);
end;

function BlendC(x, y: Integer; d: double): integer;
begin
  result := round(x*(1-d) + y*d);
end;
function RGB(r, g, b: byte): TColor;
begin
  result := r + g shl 8 + b shl 16;
end;

function GetRValue(c: TColor): Byte;
begin
  Result := c and 255;
end;
function GetGValue(c: TColor): Byte;
begin
  Result := (c shr 8) and 255;
end;
function GetBValue(c: TColor): Byte;
begin
  Result := (c shr 16) and 255;
end;

function TfrmMain.Blend(c1, c2: TColor; d: double): TColor;
begin
  Result := RGB(
         BlendC(GetRValue(c1), GetRValue(c2), d),
         BlendC(GetGValue(c1), GetGValue(c2), d),
         BlendC(GetBValue(c1), GetBValue(c2), d));
end;



procedure TfrmMain.loadAI(Sender: TObject; var Value: String);
begin
  sbResetClick(nil);
end;

procedure TfrmMain.tbStartChange(Sender: TObject);
begin
  if TToggleBox(Sender).checked then
    Disable();
  Handler.FRunning := TToggleBox(Sender).checked;
end;

procedure TfrmMain.tTerminateTimerTimer(Sender: TObject);
begin
  prAI.Active := false;
  tTerminateTimer.Enabled := false;
  executing := false;
end;

{ TPipeAIHandler }

procedure TPipeAIHandler.FinishStep();
begin
  inherited FinishStep();
  frmMain.RefreshScores();
  frmMain.Draw;
  if not FRunning then
  begin
    frmMain.tbStart.Checked := false;
    frmMain.Enable();
    Enabled := true;
  end
  else
    frmMain.Disable();
end;

procedure TPipeAIHandler.Redraw();
begin
  inherited Redraw();
  frmMain.RefreshScores();
  frmMain.Draw;
end;

procedure TPipeAIHandler.ShowFinalMessage();
var
  i: Integer;
  maxs1, maxs2: Integer;
  s1, s2: String;
begin
  inherited ShowFinalMessage();
  maxs1 := 0;
  maxs2 := 0;
  s1 := '';
  s2 := '';
  for i := 1 to PlayersCount do
  begin
    maxs1 := max(maxs1, Scores[i]);
    maxs2 := max(maxs2, Influence[i]);
  end;
  for i := 1 to PlayersCount do
  begin
    if Scores[i] = maxs1 then
      s1 := s1 + ' Player' + IntToStr(i) + ';';
    if Influence[i] = maxs2 then
      s2 := s2 + ' Player' + IntToStr(i) + ';';
  end;
  ShowMessage('1. Очки: '+IntToStr(maxs1) + ',' + s1 + #10 + '2. Захвачено территории: ' +
    IntToStr(maxs2) + ',' + s2);
end;

procedure TPipeAIHandler.SetEnabled(enbl: Boolean);
begin
  inherited SetEnabled(enbl);
  if (not FRunning) and (Enabled) then
  begin
    frmMain.tbStart.Checked := false;
    frmMain.Enable();
    Enabled := true;
  end
  else
    frmMain.Disable();
end;

initialization
  {$I main.lrs}

end.

