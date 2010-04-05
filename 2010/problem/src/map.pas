unit map;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Dialogs;

type

  { TMap }

  TMap = class
  private
    f: array[0..1000, 0..1000] of Integer;
    c: array[0..1000, 0..1000] of Integer;
    b: array[0..1000, 0..1000] of Boolean;
    v: array[0..1000, 0..1000] of Boolean;
    s1: array[0..10] of Integer;
    s2: array[0..10] of Integer;
    w, h, p, l, cp, st: Integer;
    go: boolean;
    function GetField(x, y: Integer): Integer;
    function GetColor(x, y: Integer): Integer;
    function GetBases(x, y: Integer): Boolean;
    procedure Fill(x, y, z: Integer);
    function GetCP: Integer;
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
    property CurrentPlayer: Integer read GetCP;
    property GameOver: Boolean read go;
    property StepsPassed: Integer read st write st;
    procedure ResetValues();
    procedure CalculateScores();

    function Step(x, y, d: Integer): Integer;

    procedure Load(fname: string);
    procedure Save(fname: string);
    procedure ProcessAIOutput(fname: string; id: Integer);

  end;

implementation

{ TMap }

procedure TMap.Fill(x, y, z: Integer);
begin
  if ((x < 0) or (x >= w)) or
     ((y < 0) or (y >= h)) then
    exit;
  if ((Field[x,y] and z) = 0) then
    exit;
  if (Bases[x, y]) and (Color[x, y] <> CurrentPlayer) then
    exit;
  if (v[x, y]) then
    exit;
  c[x, y] := CurrentPlayer;
  v[x, y] := true;
  if (Field[x, y] and 2) <> 0 then
    Fill(x, y + 1, 8);
  if (Field[x, y] and 4) <> 0 then
    Fill(x + 1, y, 1);
  if (Field[x, y] and 8) <> 0 then
    Fill(x, y - 1, 2);
  if (Field[x, y] and 1) <> 0 then
    Fill(x - 1, y, 4);
end;

function TMap.GetCP: Integer;
begin
  Result := cp + 1;
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
end;

procedure TMap.CalculateScores();
var
  i, j: Integer;
begin
  for i := 0 to 10 do
  begin
    s2[i] := 0;
  end;
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      inc(s1[c[i,j]]);
      inc(s2[c[i,j]]);
    end;
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
    end;
  st := 0;
end;


function TMap.Step(x, y, d: Integer): Integer;
var
  i, j: integer;
begin
  Result := -1;
  if (((x < 1) or (x > w)) or
     (((y < 1) or (y > h)) or
      ((d < 0) or (d > 3)))) or
       (b[x - 1, y - 1]) then
    exit;
  dec(x);
  dec(y);
  f[x, y] := (f[x, y] shr d) or (f[x, y] shl (4 - d));
  for i := 0 to w - 1 do
    for j := 0 to h - 1 do
      v[i, j] := false;
  Fill(x, y, 15);
  b[x, y] := true;
  cp := (cp + 1) mod PlayersCount;
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
    end;
  closefile(fi);
end;

procedure TMap.Save(fname: string);
var
  fo: TextFile;
  i, j: Integer;
begin
  assignfile(fo, fname);
  rewrite(fo);
  writeln(fo, w, ' ', h, ' ', p, ' ', l);
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      write(fo, Color[i, j]);
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

