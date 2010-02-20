object frmMain: TfrmMain
  Left = 0
  Top = 0
  Caption = 'frmMain'
  ClientHeight = 718
  ClientWidth = 851
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object pbMain: TPaintBox
    Left = 0
    Top = 89
    Width = 851
    Height = 629
    Align = alClient
    OnMouseDown = pbMainMouseDown
    OnMouseMove = pbMainMouseMove
    OnMouseUp = pbMainMouseUp
    OnPaint = pbMainPaint
    ExplicitLeft = 328
    ExplicitTop = 312
    ExplicitWidth = 105
    ExplicitHeight = 105
  end
  object pTools: TPanel
    Left = 0
    Top = 0
    Width = 851
    Height = 89
    Align = alTop
    Caption = 'pTools'
    TabOrder = 0
    object lTurn: TLabel
      Left = 376
      Top = 8
      Width = 62
      Height = 13
      Caption = 'Player 1 turn'
    end
    object seW: TSpinEdit
      Left = 8
      Top = 8
      Width = 121
      Height = 22
      MaxValue = 100
      MinValue = 1
      TabOrder = 0
      Value = 10
    end
    object seH: TSpinEdit
      Left = 8
      Top = 36
      Width = 121
      Height = 22
      MaxValue = 100
      MinValue = 1
      TabOrder = 1
      Value = 10
    end
    object sePC: TSpinEdit
      Left = 144
      Top = 8
      Width = 121
      Height = 22
      MaxValue = 6
      MinValue = 1
      TabOrder = 2
      Value = 2
    end
    object brnStart: TButton
      Left = 280
      Top = 6
      Width = 75
      Height = 25
      Caption = 'Start'
      TabOrder = 3
      OnClick = brnStartClick
    end
    object lbScores: TListBox
      Left = 528
      Top = 1
      Width = 322
      Height = 87
      Align = alRight
      ItemHeight = 13
      TabOrder = 4
    end
    object cbEdit: TCheckBox
      Left = 144
      Top = 36
      Width = 97
      Height = 17
      Caption = 'Edit'
      TabOrder = 5
    end
    object tbCellsize: TTrackBar
      Left = 144
      Top = 59
      Width = 378
      Height = 42
      Max = 64
      Min = 8
      Position = 8
      TabOrder = 6
      OnChange = tbCellsizeChange
    end
  end
end
