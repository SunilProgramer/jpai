unit aihandler;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, FileUtil, Controls, Dialogs, Forms, ExtCtrls, EditBtn, Graphics, process, AsyncProcess,
  playerframe, map;

const
  PLAYER_COLORS: array[0..9] of TColor = (clBlack, clRed, clBlue, clGreen, clYellow, clMaroon, clLime, clAqua, clPurple, clTeal);
  min_count = 2;
  max_count = 9;
  AllowedWorkTime = 3000;
  DefaultAI = 'random';
  AIExecutable = 'ai';

type

  { TPlayer }

  TPlayer = class
    private
      frm: TfPlayer;
      num: Integer;
    public
      constructor Create(n: Integer; AI: String; Parent: TWinControl);
      property Form: TfPlayer read frm write frm;
      procedure Prepare(destdir: string; tempdir: string);
      destructor Destroy();
  end;

  { TAIHandler }

  TAIHandler = class
  private
    AProcess: TAsyncProcess;//process to run ai
    timer: TTimer;//kill timer
    p: array of TPlayer;//players
    count: Integer;//player count
    m: TMap;//map
    owner: TComponent;//owner
    apath: string;//application path
    rundir: string;//ai run directory
    tempdir: string;//temporary directory
    running: boolean;//indicates whether program is running
    aiqueue: TList;//queue of ai to run
    sleft: Integer;// Steps Left
    en: boolean;
    function GetEnabled: Boolean;
    function GetPlayer(i: Integer): TPlayer;
    procedure SetPlayer(i: Integer; player: TPlayer);
    procedure SetAcceptEvent(event: TAcceptFileNameEvent);
    procedure BeginAI();
    procedure EndAI(Sender: TObject);
    procedure RunAI();
    procedure StoreQueue();
    function gets1(player: integer): integer;
    function gets2(player: integer): integer;
    procedure SetState(run: Boolean);
    procedure SetStepsCount(cnt: Integer);
    procedure SetEnabled(enbl: Boolean);
  protected
    procedure FinishStep(); virtual;
    procedure ShowFinalMessage(); virtual;
  public
    property Players[i: Integer]: TPlayer read GetPlayer write SetPlayer;
    property PlayersCount: Integer read count;
    property AcceptEvent: TAcceptFileNameEvent write SetAcceptEvent;
    property Map: TMap read m write m;
    property AppPath: String read apath;
    property Scores[i: Integer]: Integer read gets1;
    property Influence[i: Integer]: Integer read gets2;
    property FRunning: Boolean read running write SetState;
    property StepsLeft: Integer read sleft write SetStepsCount;
    property Enabled: Boolean read GetEnabled write SetEnabled;
    Function GameOver: Boolean;
    function AddPlayer(AI: string): Boolean;
    procedure MakeStep();
    function RemoveLastPlayer(): Boolean;
    procedure CalculateScores();
    procedure Refresh();

    constructor Create(AOwner: TComponent);
  end;

implementation


{ TAIHandler }

function TAIHandler.GetPlayer(i: Integer): TPlayer;
begin
  //may be throw exception
  Result := p[i];
end;

function TAIHandler.GetEnabled: Boolean;
begin
  Result := en and (not running);
end;


procedure TAIHandler.SetPlayer(i: Integer; player: TPlayer);
begin
  // may be throw exception
  p[i] := player;
end;

procedure TAIHandler.SetAcceptEvent(event: TAcceptFileNameEvent);
var
  i: Integer;
begin
  for i := 0 to count - 1 do
    p[i].frm.fneAI.OnAcceptFileName := event;
end;

procedure TAIHandler.EndAI(Sender: TObject);
var
  player: Integer;
  i: Integer;
begin
  AProcess.Terminate(0);
  timer.Enabled := false;
  if sender = timer then
  begin
    exit;
  end;
  player := Integer(aiqueue[0]);
  aiqueue.Delete(0);
  m.ProcessAIOutput(rundir + DirectorySeparator + 'output.txt', player);
  if FileExists(rundir + DirectorySeparator + 'temp.txt') then
  begin
    CopyFile(rundir + DirectorySeparator + 'temp.txt', tempdir +
      DirectorySeparator + 'temp' + IntToStr(player)+'.txt');
    DeleteFile(rundir + DirectorySeparator + 'temp.txt');
  end;
  if aiqueue.Count = 0 then
  begin
    if GameOver then
    begin
      ShowFinalMessage();
      FRunning := false;
      FinishStep();
      exit;
    end;
//    Enabled := not running;
    FinishStep();
    if running then
      StoreQueue()
    else
      exit;
  end;
  BeginAI();
end;

procedure TAIHandler.StoreQueue();
var
  i: Integer;
begin
  for i := 0 to PlayersCount - 1 do
  begin
    aiqueue.Add(Pointer(i));
    if not FileExists(p[i].frm.fneAI.FileName) then
    begin
      showmessage('Пожалуйста выберите файл искусственного интеллекта для Игрока ' +
        IntToStr(i+1) + '.');
      running := false;
      aiqueue.Clear;
      exit;
    end;
  end;
end;

function TAIHandler.gets1(player: integer): integer;
begin
  Result := m.Scores1[player];
end;

function TAIHandler.gets2(player: integer): integer;
begin
  Result := m.Scores2[player];
end;

procedure TAIHandler.SetState(run: Boolean);
var
  i: Integer;
begin
  running := run;
  if run then
    RunAI();
end;

procedure TAIHandler.SetStepsCount(cnt: Integer);
var
  s: string;
begin
  s := '';
  sleft := cnt;
  p[0].frm.fneAI.OnAcceptFileName(nil, s);
end;


procedure TAIHandler.SetEnabled(enbl: Boolean);
var
  i: Integer;
begin
  en := enbl;
  for i := 0 to count - 1 do
    if p[i].Form.fneAI.ReadOnly <> not Enabled then
      p[i].Form.fneAI.ReadOnly := not Enabled;
end;

procedure TAIHandler.FinishStep();
begin
  if not GameOver then
  begin
    dec(m.StepsLeft);
    inc(m.StepsPassed);
  end;
  Enabled := not running;
  CalculateScores();
end;

procedure TAIHandler.ShowFinalMessage();
begin

end;

function TAIHandler.GameOver: Boolean;
begin
  Result := (m.StepsLeft = 0) or m.Full;
end;

procedure TAIHandler.RunAI();
begin
  if (aiqueue.Count <> 0) then
    exit;
  if GameOver then
  begin
    ShowFinalMessage();
    FRunning := false;
//    Enabled := true;
    FinishStep();
    exit;
  end;
  StoreQueue();
  BeginAI();
end;


procedure TAIHandler.BeginAI();
var
  player: Integer;
begin
  if aiqueue.Count = 0 then
    exit;
  Enabled := false;
  player := Integer(aiqueue[0]);
  p[player].Prepare(rundir, tempdir);
  m.CurrentPlayer := player;
  DeleteFile(rundir + DirectorySeparator + 'output.txt');
  m.Save(rundir + DirectorySeparator + 'input.txt');
  timer.Enabled := true;
  AProcess.Execute();
  //mb remove output.txt
end;

function TAIHandler.AddPlayer(AI: string): Boolean;
begin
  Result := false;
  if count >= max_count then
    exit;
  SetLength(p, count + 1);
  p[count] := TPLayer.Create(count + 1, AI, TWinControl(owner));
  p[count].frm.fneAI.OnAcceptFileName := p[0].frm.fneAI.OnAcceptFileName;
  inc(count);
  m.PlayersCount := count;
  Result := true;
end;

procedure TAIHandler.MakeStep();
begin
  running := false;
  RunAI();
end;

function TAIHandler.RemoveLastPlayer(): Boolean;
begin
  Result := false;
  if count <= min_count then
    exit;
  p[count - 1].Destroy;
  dec(count);
  SetLength(p, count);
  Result := true;
end;

procedure TAIHandler.CalculateScores();
var
   i: Integer;
begin
  m.CalculateScores();
  for i := 0 to PlayersCount - 1 do
  begin
    p[i].Form.leInfluence.Text := inttostr(Influence[i + 1]);
    p[i].Form.leScores.Text := inttostr(Scores[i + 1]);
  end;
end;

procedure TAIHandler.Refresh();
begin
  m.ResetValues();
  map.StepsLeft := StepsLeft;
  CalculateScores();
end;

constructor TAIHandler.Create(AOwner: TComponent);
var
  i: Integer;
begin
  apath := ExtractFileDir(Application.ExeName);
  rundir := apath + DirectorySeparator + 'runarea';
  tempdir := apath + DirectorySeparator + 'temp';
  AProcess := TAsyncProcess.Create(AOwner);
  AProcess.CommandLine := rundir + DirectorySeparator + AIExecutable;
  AProcess.CurrentDirectory := rundir;
  AProcess.OnTerminate := @EndAI;
  AProcess.Options := [poNoConsole];
  AProcess.ShowWindow := swoHIDE;
  timer := TTimer.Create(AOwner);
  timer.Enabled := false;
  timer.Interval := AllowedWorkTime;
  timer.OnTimer := @EndAI;
  m := TMap.Create();
  aiqueue := TList.Create();
  en := true;
  running := false;
  owner := AOwner;
  count := 0;
  for i :=1 to min_count do
    AddPlayer(DefaultAI);
end;

{ TPlayer }

constructor TPlayer.Create(n: Integer; AI: String; Parent: TWinControl);
begin
  num := n;
  frm := TfPlayer.Create(Parent);
  frm.Parent := Parent;
  frm.Align := alTop;
  frm.Top := MaxInt;
  frm.Name := frm.Name + IntToStr(n);
  frm.fneAI.Font.Color := PLAYER_COLORS[n];
  frm.fneAI.InitialDir := ExtractFileDir(Application.ExeName) + DirectorySeparator + 'ai';
  frm.fneAI.FileName := frm.fneAI.InitialDir + DirectorySeparator + AI + GetExeExt;
  frm.Tag := n;
end;

procedure TPlayer.Prepare(destdir: string; tempdir: string);
begin
  if not DirectoryExists(destdir) then
    raise Exception.Create('WTF?'); //check this out
  CopyFile(Form.fneAI.FileName, destdir + DirectorySeparator + 'ai'+GetExeExt);
  {$IFDEF UNIX}// changing file attributes
  FpChmod(dir + DirectorySeparator + 'ai', &775);
  {$ENDIF}
  CheckIfFileIsExecutable(destdir + DirectorySeparator + 'ai'+GetExeExt);// mb delete this
  if FileExists(tempdir + DirectorySeparator + 'temp' + IntToStr(Form.Tag)) then
    CopyFile(tempdir + DirectorySeparator + 'temp' + IntToStr(Form.Tag)+'.txt',
      destdir + DirectorySeparator + 'temp.txt');
end;

destructor TPlayer.Destroy();
begin
  frm.Destroy();
end;

end.

