unit map;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Dialogs, log, math;

type

  { TMap }

  TMap = class
  private
    f: array[0..1000, 0..1000] of Integer;
    fr: array[0..1000, 0..1000] of Integer;
    c: array[0..1000, 0..1000] of Integer;
    cr: array[0..1000, 0..1000] of Integer;
    b: array[0..1000, 0..1000] of Boolean;
    v: array[0..1000, 0..1000] of Boolean;
    vt: array[0..1000, 0..1000] of Integer;
    vtr: array[0..1000, 0..1000] of Integer;
    sc: array[0..100] of Integer;
    s1: array[0..10] of Integer;
    s2: array[0..10] of Integer;
    w, h, p, l, cp, st, mv: Integer;
    go: boolean;
    fl: Boolean;
    logfile: TLog;
    function GetField(x, y: Integer): Integer;
    function GetInfluence(x, y: Integer): Integer;
    function GetColor(x, y: Integer): Integer;
    function GetBases(x, y: Integer): Boolean;
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

function TMap.Fill(x, y, d, z: Integer): Integer;
begin
  Result := 0;
  if ((x < 0) or (x >= w)) or
     ((y < 0) or (y >= h)) then
    exit;
  if (v[x, y]) then
    exit;
  if (c[x, y] = CurrentPlayer) and (z = 2) then
    exit;
  if (z < 2) then
  begin
    if (c[x, y] = CurrentPlayer) and (z = 1) then
    begin
        vt[x, y] := vt[x, y] + 1;
        v[x, y] := true;
    end
    else
        if (((vt[x, y]< d) and (c[x, y] <> CurrentPlayer)) and (c[x,y] <> 0)) and not (v[x, y]) then
        begin
           c[x, y] := CurrentPlayer;
           Result := Result + 1;
           v[x, y] := true;
        end
        else
            exit;
  end
  else
  begin
    c[x, y] := CurrentPlayer;
    v[x, y] := true;
  end;
  d := vt[x, y];
  Result := Result + Fill(x - 1, y, d, z-1);
  Result := Result + Fill(x + 1, y, d, z-1);
  Result := Result + Fill(x - 1 + y mod 2, y - 1, d, z-1);
  Result := Result + Fill(x + y mod 2, y - 1, d, z-1);
  Result := Result + Fill(x - 1 + y mod 2, y + 1, d, z-1);
  Result := Result + Fill(x + y mod 2, y + 1, d, z-1);
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
      inc(s2[c[i,j]]);
      if (c[i, j] <> 0) then
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
      c[i, j] := cr[i,j];
      f[i, j] := fr[i, j];
      vt[i, j] := vtr[i, j];
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
     ((y < 1) or (y > h))) or
       (c[x - 1, y - 1] <> 0)
    then
  begin
//    cp := (cp + 1) mod PlayersCount;
    exit;
  end;
  dec(x);
  dec(y);
  d := min(d, mv);
  for i := 0 to w - 1 do
  begin
      for j := 0 to h - 1 do
        v[i, j] := false;
  end;

  vt[x, y] := d;
  //do: decrement count of d cards

  i := Fill(x, y, d, 2);
  logfile.Message('Player'+IntToStr(cp + 1) + ' - (' + IntToStr(x+1) + ',' +
    IntToStr(y+1) + ') - ' + IntToStr(d)+' = '+inttostr(i));
  b[x, y] := true;
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
  read(fi, w, h, mv);
  ResetValues();
  mv := 20;
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
  writeln(fo, w, ' ', h, ' ', p, ' ', l-1);
{  for j := 0 to h - 1 do
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
    end;}
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

