unit log;

{$mode objfpc}{$H+}

interface

uses
  Classes, SysUtils, Forms;

type

  { TLog }

  TLog = class
  private
    fname: String;
    fl: TextFile;
  public
    constructor Create(fn: string = 'log.txt');
    procedure Clear();
    destructor Destroy(); override;
    procedure Message(msg: String);
  end;

implementation

{ TLog }

constructor TLog.Create(fn: string);
begin
  fname := ExtractFileDir(Application.ExeName) + DirectorySeparator + fn;
  Clear();
end;

procedure TLog.Clear();
begin
  AssignFile(fl, fname);
  rewrite(fl);
  CloseFile(fl);
end;

destructor TLog.Destroy();
begin
  inherited Destroy();
  CloseFile(fl);
end;

procedure TLog.Message(msg: String);
begin
  AssignFile(fl, fname);
  Append(fl);
  Writeln(fl, msg);
  CloseFile(fl);
end;

end.

