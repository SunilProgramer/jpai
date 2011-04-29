unit map;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Dialogs, log, math;

const
  MAX_SIZE = 3000;

type

  { TMap }

  TMap = class
  private
    f: array[0..MAX_SIZE, 0..MAX_SIZE] of Integer;
    fr: array[0..MAX_SIZE, 0..MAX_SIZE] of Integer;
    c: array[0..MAX_SIZE, 0..MAX_SIZE] of Integer;
    cr: array[0..MAX_SIZE, 0..MAX_SIZE] of Integer;
    vt: array[0..MAX_SIZE, 0..MAX_SIZE] of Integer;
    vtr: array[0..MAX_SIZE, 0..MAX_SIZE] of Integer;
    seq: array[1..10, 0..MAX_SIZE] of Integer;
    sc: array[0..100] of Integer;
    s1: array[0..10] of Integer;
    s2: array[0..10] of Integer;
    w, h, p, l, cp, st, mv, seqsz, wsz: Integer;
    go: boolean;
    fl: Boolean;
    logfile: TLog;
    function GetField(x, y: Integer): Integer;
    function GetInfluence(x, y: Integer): Integer;
    function GetColor(x, y: Integer): Integer;
    function Fill(x, y, d, z: Integer): Integer;
    function GetCP: Integer;
    procedure SetCp(pl: Integer);
    function gets1(player: integer): integer;
    function gets2(player: integer): integer;
  public
    constructor Create();
    property Width: Integer read w;
    property MaxValue: Integer read mv;
    property Height: Integer read h;
    property Field[x, y: Integer]: Integer read GetField;
    property Influence[x, y: Integer]: Integer read GetInfluence;
    property Color[x, y: Integer]: Integer read GetColor;
    property Scores1[player: Integer]: Integer read gets1;
    property Scores2[player: Integer]: Integer read gets2;
    property PlayersCount: Integer read p write p;
    property StepsLeft: Integer read l write l;
    property CurrentPlayer: Integer read GetCP write SetCP;
    property GameOver: Boolean read go;
    property StepsPassed: Integer read st write st;
    property Full: Boolean read fl;
    procedure ResetValues();
    procedure CalculateScores();
    function NextNumber(): integer;

    function Step(x, y: Integer): Integer;

    procedure Load(fname: string);
    procedure Save(fname: string);
    procedure ProcessAIOutput(fname: string; id: Integer);

  end;

implementation

{ TMap }

function TMap.Fill(x, y, d, z: Integer): Integer;
begin
  Result := 0;
  if ((x < 0) or (x >= w)) or
     ((y < 0) or (y >= h)) then
    exit;
  if (z = 0) then
  begin
    if c[x, y] = CurrentPlayer then
        vt[x, y] := min(vt[x, y] + 1, mv)
    else
        if ((vt[x, y]< d) and (c[x, y] <> CurrentPlayer)) and (c[x,y] <> 0) then
        begin
           c[x, y] := CurrentPlayer;
           Result := Result + 1;
        end;
    exit;
  end
  else
    c[x, y] := CurrentPlayer;
  d := vt[x, y];
  Result := Result + Fill(x - 1, y, d, 0);
  Result := Result + Fill(x + 1, y, d, 0);
  Result := Result + Fill(x - 1 + y mod 2, y - 1, d, 0);
  Result := Result + Fill(x + y mod 2, y - 1, d, 0);
  Result := Result + Fill(x - 1 + y mod 2, y + 1, d, 0);
  Result := Result + Fill(x + y mod 2, y + 1, d, 0);
end;

function TMap.GetCP: Integer;
begin
  Result := cp + 1;
end;

procedure TMap.SetCp(pl: Integer);
begin
  cp := pl;
end;

function TMap.gets1(player: integer): integer;
begin
  Result := s1[player];
end;

function TMap.gets2(player: integer): integer;
begin
  Result := s2[player];
end;

constructor TMap.Create();
begin
  go := false;
  logfile := TLog.Create;
end;

procedure TMap.CalculateScores();
var
  i, j, s: Integer;
begin
  for i := 0 to 10 do
  begin
    s2[i] := 0;
    s1[i] := 0;
    inc(s1[i], sc[i]);
    sc[i] := 0;
  end;
  s := 0;
  fl := false;
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      inc(s1[c[i,j]]);
      sc[c[i,j]] := s1[c[i,j]];
      inc(s2[c[i,j]], vt[i, j]);
      if (c[i, j] <> 0) then
        inc(s);
    end;
  if s = w*h then
    fl := true;
end;

function TMap.NextNumber(): integer;
begin
  Result := seq[CurrentPlayer, StepsPassed mod seqsz];
end;

procedure TMap.ResetValues();
var
  i, j: Integer;
begin
  for i := 0 to 10 do
  begin
    s1[i] := 0;
    s2[i] := 0;
  end;
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      c[i, j] := cr[i,j];
      f[i, j] := fr[i, j];
      vt[i, j] := vtr[i, j];
    end;
  st := 0;
  fl := false;
  logfile.Clear();
end;


function TMap.Step(x, y: Integer): Integer;
var
  i: integer;
begin
  Result := -1;
  sc[cp + 1] := 0;
  if (((x < 1) or (x > w)) or
     ((y < 1) or (y > h))) or
       (c[x - 1, y - 1] <> 0)
    then
  begin
//    cp := (cp + 1) mod PlayersCount;
    exit;
  end;
  dec(x);
  dec(y);

  vt[x, y] := seq[CurrentPlayer, StepsPassed mod seqsz];
  //do: decrement count of d cards

  i := Fill(x, y, vt[x, y], 1);
  logfile.Message('Player'+IntToStr(cp + 1) + ' - (' + IntToStr(x+1) + ',' +
    IntToStr(y+1) + ') - ' + IntToStr(vt[x, y])+' = '+inttostr(i));
  //cp := (cp + 1) mod PlayersCount;
end;

function TMap.GetField(x, y: Integer): Integer;
begin
  Result := f[x, y];
end;

function TMap.GetInfluence(x, y: Integer): Integer;
begin
  Result := vt[x, y];
end;

function TMap.GetColor(x, y: Integer): Integer;
begin
  Result := c[x, y];
end;

procedure TMap.Load(fname: string);
var
  fi: TextFile;
  i, j: Integer;
begin
  assignfile(fi, fname);
  reset(fi);
  read(fi, w, h, mv, wsz, seqsz);// width height max value windowsize count

  ResetValues();
  for i := 1 to 10 do
      for j := 0 to seqsz - 1 do
            read(fi, seq[i, j]);
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      read(fi, c[i, j]);
      read(fi, vt[i, j]);
      if (c[i, j] = 0) then
         vt[i, j] := 0;
      vt[i,j] := min(vt[i, j], mv);
      cr[i, j] := c[i, j];
      vtr[i, j] := vt[i, j];
    end;
  closefile(fi);
end;

procedure TMap.Save(fname: string);
var
  fo: TextFile;
  i, j, col: Integer;
begin
  assignfile(fo, fname);
  rewrite(fo);
  writeln(fo, w, ' ', h, ' ', wsz, ' ', PlayersCount);
  for i := 0 to wsz-1 do
      write(fo, seq[CurrentPlayer, (StepsPassed + i) mod seqsz], ' ');
  writeln(fo);
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      col := 0;
      if Color[i, j] <> 0 then
        col := (Color[i, j] - 1 + PlayersCount - cp) mod PlayersCount + 1;
      write(fo, col, ' ');
      write(fo, Influence[i, j], '  ');
      if (i <> w - 1) then
        write(fo, ' ')
      else
        writeln(fo);
    end;
  closefile(fo);
end;

procedure TMap.ProcessAIOutput(fname: string; id: Integer);
var
  fi: TextFile;
  x, y: Integer;
begin
  if not FileExists(fname) then
     exit;
  assignfile(fi, fname);
  reset(fi);
  try
    read(fi, x, y);
    Step(x, y);
  finally
    closefile(fi);
  end;
end;

end.

