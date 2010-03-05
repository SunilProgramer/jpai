unit map;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils;

type

  { TMap }

  TMap = class
  private
    f: array[0..1000, 0..1000] of Integer;
    c: array[0..1000, 0..1000] of Integer;
    b: array[0..1000, 0..1000] of Boolean;
    w, h, p, l, cp: Integer;
    function GetField(x, y: Integer): Integer;
    function GetColor(x, y: Integer): Integer;
    function GetBases(x, y: Integer): Boolean;
    function GetHeight: Integer;
    function GetWidth: Integer;
    procedure Fill(x, y: Integer);
  public
    property Width: Integer read GetWidth;
    property Height: Integer read GetHeight;
    property Field[x, y: Integer]: Integer read GetField;
    property Color[x, y: Integer]: Integer read GetColor;
    property Bases[x, y: Integer]: Boolean read GetBases;
    property PlayersCount: Integer read p write p;
    property StepsLeft: Integer read l write l;
    property CurrentPlayer: Integer read cp;

    function Step(x, y, d: Integer): Integer;

    procedure Load(fname: string);
    procedure Save(fname: string);

  end;

implementation

{ TMap }

function TMap.GetWidth: Integer;
begin
  Result := w;
end;

procedure TMap.Fill(x, y: Integer);
begin
  if ((x < 0) or (x >= w)) or
     ((y < 0) or (y >= h)) then
    exit;
  if (Bases[x, y]) and (Color[x, y] <> CurrentPlayer) then
    exit;
  c[x, y] := CurrentPlayer;
  if Field[x, y] and 1 <> 0 then
    Fill(x, y - 1);
  if Field[x, y] and 2 <> 0 then
    Fill(x + 1, y);
  if Field[x, y] and 4 <> 0 then
    Fill(x, y + 1);
  if Field[x, y] and 8 <> 0 then
    Fill(x - 1, y);
end;

function TMap.Step(x, y, d: Integer): Integer;
begin
  Result := -1;
  if ((x < 0) or (x >= w)) or
    (((y < 0) or (y >= h)) or
     ((d < 0) or (d > 3))) then
    exit;
  f[x, y] := (f[x, y] shl d) or (f[x, y] shr (3 - d));
  Fill(x, y);
  b[x, y] := true;
  cp := (cp + 1) mod PlayersCount;
end;

function TMap.GetHeight: Integer;
begin
  Result := h;
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
  for j := 0 to h - 1 do
    for i := 0 to w - 1 do
    begin
      read(fi, f[i, j]);
      b[i, j] := false;
      c[i, j] := 0;
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
      write(fo, Field[i, j] and 1);
      write(fo, (Field[i, j] and 2) shr 1);
      write(fo, (Field[i, j] and 4) shr 2);
      write(fo, (Field[i, j] and 8) shr 3);
      if (i <> w - 1) then
        write(fo, ' ')
      else
        writeln(fo);
    end;
  closefile(fo);
end;

end.

