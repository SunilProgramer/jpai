unit map;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Dialogs, log;

type

  { TMap }

  TMap = class
  private
    f: array[0..1000, 0..1000] of Integer;
    fr: array[0..1000, 0..1000] of Integer;
    c: array[0..1000, 0..1000] of Integer;
    b: array[0..1000, 0..1000] of Boolean;
    v: array[0..1000, 0..1000] of Boolean;
    sc: array[0..100] of Integer;
    s1: array[0..10] of Integer;
    s2: array[0..10] of Integer;
    w, h, p, l, cp, st: Integer;
    go: boolean;
    fl: Boolean;
    logfile: TLog;
    function GetField(x, y: Integer): Integer;
    function GetColor(x, y: Integer): Integer;
    function GetBases(x, y: Integer): Boolean;
    function Fill(x, y, z: Integer): Integer;
    function GetCP: Integer;
    procedure SetCp(pl: Integer);
    function gets1(player: integer): integer;
    function gets2(player: integer): integer;
  public
    constructor Create();
    property Width: Integer read w;
    property Height: Integer read h;
    property Field[x, y: Integer]: Integer read GetField;
    property Color[x, y: Integer]: Integer read GetColor;
    property Bases[x, y: Integer]: Boolean read GetBases;
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

    function Step(x, y, d: Integer): Integer;

    procedure Load(fname: string);
    procedure Save(fname: string);
    procedure ProcessAIOutput(fname: string; id: Integer);

  end;

implementation

{ TMap }

function TMap.Fill(x, y, z: Integer): Integer;
begin
  Result := 0;
  if ((x < 0) or (x >= w)) or
     ((y < 0) or (y >= h)) then
    exit;
  if ((Field[x,y] and z) = 0) then
    exit;
  if (Color[x, y] <> 0) and (Color[x, y] <> CurrentPlayer) then
    exit;
  if (v[x, y]) then
    exit;
//  if (c[x, y] = CurrentPlayer) or (c[x, y] = 0) then
  Result := 1;
  c[x, y] := CurrentPlayer;
  v[x, y] := true;
  if (Field[x, y] and 2) <> 0 then
    Result := Result + Fill(x, y + 1, 8);
  if (Field[x, y] and 4) <> 0 then
    Result := Result + Fill(x + 1, y, 1);
  if (Field[x, y] and 8) <> 0 then
    Result := Result + Fill(x, y - 1, 2);
  if (Field[x, y] and 1) <> 0 then
    Result := Result + Fill(x - 1, y, 4);
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
    inc(s1[i], sc[i]);
    sc[i] := 0;
  end;
  s := 0;
  fl := false;
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      //inc(s1[c[i,j]]);
      inc(s2[c[i,j]]);
      if (b[i, j]) or (f[i,j]=0) then
        inc(s);
    end;
  if s = w*h then
    fl := true;
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
      b[i, j] := false;
      c[i, j] := 0;
      f[i, j] := fr[i, j];
    end;
  st := 0;
  fl := false;
  logfile.Clear();
end;


function TMap.Step(x, y, d: Integer): Integer;
var
  i, j: integer;
begin
  Result := -1;
  sc[cp + 1] := 0;
  if (((x < 1) or (x > w)) or
     (((y < 1) or (y > h)) or
      ((d < 0) or (d > 3)))) or
       (b[x - 1, y - 1]) or (f[x - 1, y - 1] = 0)
    then
  begin
//    cp := (cp + 1) mod PlayersCount;
    exit;
  end;
  dec(x);
  dec(y);
  f[x, y] := (f[x, y] shr d) or (f[x, y] shl (4 - d));
  for i := 0 to w - 1 do
    for j := 0 to h - 1 do
      v[i, j] := false;
  if c[x, y] <> cp + 1 then
    c[x, y] := 0;
  sc[cp + 1] := Fill(x, y, 15);
  logfile.Message('Player'+IntToStr(cp + 1) + ' - (' + IntToStr(x+1) + ',' +
    IntToStr(y+1) + ') - ' + IntToStr(d*90)+' = ' + IntToStr(sc[cp + 1]));
  b[x, y] := true;
  //cp := (cp + 1) mod PlayersCount;
end;

function TMap.GetField(x, y: Integer): Integer;
begin
  Result := f[x, y];
end;

function TMap.GetColor(x, y: Integer): Integer;
begin
  Result := c[x, y];
end;

function TMap.GetBases(x, y: Integer): Boolean;
begin
  Result := b[x, y];
end;

procedure TMap.Load(fname: string);
var
  fi: TextFile;
  i, j: Integer;
begin
  assignfile(fi, fname);
  reset(fi);
  read(fi, w, h);
  ResetValues();
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      read(fi, f[i, j]);
      fr[i, j] := f[i, j];
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
  writeln(fo, w, ' ', h, ' ', p, ' ', l-1);
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      col := 0;
      if Color[i, j] <> 0 then
        col := (Color[i, j] - 1 + PlayersCount - cp) mod PlayersCount + 1;
      write(fo, col);
      write(fo, Integer(Bases[i, j]));
      write(fo, (Field[i, j] and 8) shr 3);
      write(fo, (Field[i, j] and 4) shr 2);
      write(fo, (Field[i, j] and 2) shr 1);
      write(fo, Field[i, j] and 1);
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
  x, y, d: Integer;
begin
  if not FileExists(fname) then exit;
  assignfile(fi, fname);
  reset(fi);
  try
    read(fi, x, y, d);
    Step(x, y, d);
  finally
    closefile(fi);
  end;
end;

end.

