object Form1: TForm1
  Left = 243
  Top = 55
  Caption = 'Network Disk'
  ClientHeight = 605
  ClientWidth = 1019
  Color = clGradientInactiveCaption
  DoubleBuffered = True
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnPaint = FormPaint
  DesignSize = (
    1019
    605)
  PixelsPerInch = 96
  TextHeight = 13
  object headptr: TLabel
    Left = 64
    Top = 554
    Width = 52
    Height = 13
    AutoSize = False
    Caption = 'y'
    Color = clGradientInactiveCaption
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindow
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = [fsUnderline]
    ParentColor = False
    ParentFont = False
  end
  object Label1: TLabel
    Left = 2
    Top = 12
    Width = 36
    Height = 13
    Caption = #1095#1090#1077#1085#1080#1081
  end
  object Label2: TLabel
    Left = 2
    Top = 28
    Width = 40
    Height = 13
    Caption = #1079#1072#1087#1080#1089#1077#1081
  end
  object Label3: TLabel
    Left = 2
    Top = 44
    Width = 29
    Height = 26
    Caption = #1086#1096#1073#1082' '#1095#1090#1077#1085'.'
    WordWrap = True
  end
  object Label4: TLabel
    Left = 2
    Top = 67
    Width = 29
    Height = 26
    Caption = #1086#1096#1073#1082' '#1079#1072#1087#1089'.'
    WordWrap = True
  end
  object Label5: TLabel
    Left = 2
    Top = 96
    Width = 48
    Height = 13
    Caption = #1086#1087#1077#1088#1072#1094#1080#1103
  end
  object pb1: TPaintBox
    Left = 2
    Top = 575
    Width = 1013
    Height = 14
    ParentShowHint = False
    ShowHint = True
    OnClick = pb1Click
    OnMouseEnter = pb1MouseEnter
    OnMouseLeave = pb1MouseLeave
    OnMouseMove = pb1MouseMove
  end
  object Label6: TLabel
    Left = 2
    Top = 119
    Width = 20
    Height = 13
    Caption = #1082#1077#1096
  end
  object diskptr: TLabel
    Left = 192
    Top = 250
    Width = 3
    Height = 13
  end
  object Label7: TLabel
    Left = 2
    Top = 143
    Width = 23
    Height = 13
    Caption = #1089#1077#1090#1100
  end
  object Label8: TLabel
    Left = 2
    Top = 536
    Width = 24
    Height = 13
    Caption = #1076#1080#1089#1082
  end
  object imgpaging: TImage
    Left = 28
    Top = 536
    Width = 12
    Height = 15
    Picture.Data = {
      0954506E67496D61676589504E470D0A1A0A0000000D494844520000000A0000
      000D08060000009037FF050000000774494D4507D70306150B00583D1B6B0000
      00097048597300001EC200001EC2016ED0753E0000000467414D410000B18F0B
      FC6105000000544944415478DA63648080FF0D0D0D0CD800549C91912C85E88A
      91C4100AC1C4FFFF280A19E1D21085288A4092C81A608A19216A5025407C640D
      20365685E8CEC0A9109B5B31DC880E90DD48B4AF490B47AA452100BCEB4909BF
      6822130000000049454E44AE426082}
    Stretch = True
    Visible = False
  end
  object cacheimg: TImage
    Left = 28
    Top = 121
    Width = 16
    Height = 13
    Picture.Data = {
      0954506E67496D61676589504E470D0A1A0A0000000D49484452000000100000
      001008060000001FF3FF610000028A4944415478DA8DD25B4814511807F0FF99
      CBEEAAEB6616A5522B5444B4E8225A81E42D2F2026B6BB965458EB6AA4747F30
      578B7C0822A5174348AC4C49C94C2D21B08B2BE516FA106E370DDFDC2E1495D1
      45DD1D7766A7334BF8229B9E879973663E7EDFFFCC1C82FF8C8234B58EDEFC7D
      4F85E9603524D88B1A9B2E3BD774A2CFE713C481FB4D39175A7E8F2C19C8DCCA
      93B2E25C57E181734665DDD16C1FB2560EA42D19B05BC34DC7AA1A7A5745AD0D
      AC3F4C4EA0B9A13AF362EB9FC14501A57BE9FEEDAE5D456546F7C729104210BB
      66053ADB2E0F959C1E495B14B05BB5E6A3A74EF684E922106BAC854EABC1D870
      0DBE7FF98CAB579A688AE9C1A040E616DA7D6F9CCBB227CF382BCC2136A19102
      2A8C3D3F0C35C7E356FB5D6749F5DBD4A0409535CC7CA46267CFEAE80878041F
      F4899D0160DC69819AE7F07EF21BAEB53CCCAA6B9D712C0094EEB6DDFA976653
      7C3CAB02660511FA240705388C0FA543C3B110E780CEAE51A7EDAC3B75015075
      30D45261DBDC1D15AD01CB2B808475C963D0857378FD7813343C03D1079A6206
      2D1D1359756DB38E79604712ED6E897865CA5B19C7A9014601BC7EA417FD4058
      28417F5B24342A028902A217B87DEFEBB3D2DAA99479A0B238A4B07C9FF64E54
      0C0B8EC6673860EA1783074FBC6068457E8E1A5A8D0CBF480181A6704BB8D13D
      9D5D7FD333403212395252C0BFC9CF20062E040180B040BF530553F94F7A0E80
      177DCB61582FC02F0112FD0E3E0FD0FB481E2E3BEF492639DBF89833A5F227C3
      06094A7C65FF0A3047E33A4638442E234832F8C0D267F23F404931FA8E932FB5
      431BD842FD714DD746BD54C872200C430166E1F196958B1F8114A20879C2CD5E
      B7377A0FCDFF8594044E4BF72B6309C34FAB9C2E714699FF05DE4AE39E9EFB5D
      440000000049454E44AE426082}
    Stretch = True
    Transparent = True
    Visible = False
  end
  object delays: TLabel
    Left = 96
    Top = 51
    Width = 31
    Height = 13
    Caption = 'delays'
  end
  object imgptr: TLabel
    Left = 1
    Top = 554
    Width = 6
    Height = 13
    Caption = 'x'
  end
  object Label9: TLabel
    Left = 3
    Top = 167
    Width = 22
    Height = 13
    Caption = 'RAM'
  end
  object Label13: TLabel
    Left = 746
    Top = 590
    Width = 268
    Height = 13
    Hint = 'disksize/max'
    Alignment = taRightJustify
    AutoSize = False
    Caption = '-'
    ParentShowHint = False
    ShowHint = True
  end
  object diskhint: TLabel
    Left = 1
    Top = 590
    Width = 309
    Height = 13
    AutoSize = False
  end
  object currentadd: TLabel
    Left = 166
    Top = 96
    Width = 87
    Height = 13
    AutoSize = False
    Caption = 'cd'
  end
  object maxquadl: TLabel
    Left = 714
    Top = 561
    Width = 301
    Height = 13
    Alignment = taRightJustify
    AutoSize = False
    Caption = 'maxquadl'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBtnText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object ftpl: TLabel
    Left = 2
    Top = 186
    Width = 18
    Height = 13
    Caption = 'FTP'
  end
  object ftpimg: TImage
    Left = 29
    Top = 187
    Width = 15
    Height = 13
    Picture.Data = {
      0954506E67496D61676589504E470D0A1A0A0000000D49484452000000100000
      000B080600000076E20D390000000774494D4507D60111103A22319970350000
      00097048597300000B1200000B1201D2DD7EFC0000000467414D410000B18F0B
      FC61050000007B4944415478DA9D91DB0DC0200845613FDCC8A49AB0910EE156
      36A6F159A8A6F7C720722E20421367008B7A2C0BFB634D168948CCC718F1C3F1
      894B7108E1556C8C5901B22A00BD6F77F9BA56C03842E9A2C744090E01B5703E
      0F00D56D765E01BF76C0CC39A574F28DFA029D736ABEE4B600A9FD718C2D0036
      BA01CCA1599860D2BEBA0000000049454E44AE426082}
  end
  object reads: TEdit
    Left = 50
    Top = 10
    Width = 40
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 0
    Text = 'reads'
  end
  object writes: TEdit
    Left = 50
    Top = 26
    Width = 40
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 1
    Text = 'writes'
  end
  object rfails: TEdit
    Left = 50
    Top = 51
    Width = 40
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 2
    Text = 'rfails'
  end
  object wfails: TEdit
    Left = 50
    Top = 71
    Width = 40
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 3
    Text = 'wfails'
  end
  object current: TEdit
    Left = 50
    Top = 94
    Width = 110
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 4
    Text = 'current'
  end
  object readb: TEdit
    Left = 91
    Top = 10
    Width = 150
    Height = 16
    TabStop = False
    Alignment = taCenter
    AutoSize = False
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 5
    Text = 'readb'
  end
  object ptr: TEdit
    Left = 41
    Top = 538
    Width = 262
    Height = 13
    Margins.Left = 19
    AutoSize = False
    BevelInner = bvNone
    BevelOuter = bvNone
    BorderStyle = bsNone
    Color = clBtnFace
    DoubleBuffered = True
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Lucida Console'
    Font.Style = []
    ParentDoubleBuffered = False
    ParentFont = False
    ParentShowHint = False
    ReadOnly = True
    ShowHint = False
    TabOrder = 6
    Text = '<'#1053#1045' '#1055#1054#1044#1050#1051#1070#1063#1045#1053'>'
  end
  object cached: TEdit
    Left = 50
    Top = 117
    Width = 110
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 7
    Text = 'cached'
  end
  object cacheblocks: TEdit
    Left = 166
    Top = 117
    Width = 152
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 8
    Text = 'cacheblocks'
    OnDblClick = cacheblocksDblClick
  end
  object net: TEdit
    Left = 50
    Top = 140
    Width = 110
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 9
    Text = 'net'
  end
  object spd: TEdit
    Left = 166
    Top = 140
    Width = 152
    Height = 19
    TabStop = False
    Alignment = taCenter
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 10
    Text = 'spd'
  end
  object imgptrcheck: TCheckBox
    Left = 289
    Top = 538
    Width = 14
    Height = 14
    Caption = 'imgptrcheck'
    Checked = True
    State = cbChecked
    TabOrder = 11
  end
  object ramsize: TEdit
    Left = 50
    Top = 167
    Width = 268
    Height = 13
    TabStop = False
    Alignment = taCenter
    AutoSize = False
    BevelInner = bvNone
    BevelOuter = bvNone
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 12
    Text = 'ramsize'
  end
  object ftpe: TEdit
    Left = 50
    Top = 186
    Width = 267
    Height = 18
    TabStop = False
    Alignment = taCenter
    AutoSize = False
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ReadOnly = True
    TabOrder = 13
    Text = '-'
  end
  object pg1: TPageControl
    Left = 1
    Top = 205
    Width = 322
    Height = 327
    ActivePage = TabSheet3
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Tahoma'
    Font.Style = []
    MultiLine = True
    ParentFont = False
    TabOrder = 14
    object TabSheet2: TTabSheet
      Caption = 'FAT32'
      ImageIndex = 1
      object Button12: TButton
        Left = 84
        Top = 45
        Width = 75
        Height = 25
        Caption = #1076#1072#1084#1087
        TabOrder = 0
        OnClick = Button12Click
      end
      object Button7: TButton
        Left = 3
        Top = 14
        Width = 75
        Height = 25
        Caption = #1079#1072#1087#1080#1089#1072#1090#1100
        TabOrder = 1
        OnClick = makefatdisk
      end
      object Button9: TButton
        Left = 84
        Top = 14
        Width = 75
        Height = 25
        Caption = #1087#1088#1086#1095#1077#1089#1090#1100
        TabOrder = 2
        OnClick = Button9Click
      end
      object Button5: TButton
        Left = 3
        Top = 45
        Width = 75
        Height = 25
        Caption = 'CRC'
        Enabled = False
        TabOrder = 3
        OnClick = Button5Click
      end
      object Button13: TButton
        Left = 3
        Top = 76
        Width = 75
        Height = 25
        Caption = 'NetFAT'
        TabOrder = 4
        OnClick = Button13Click
      end
    end
    object TabSheet3: TTabSheet
      Caption = 'debug'
      ImageIndex = 2
      object debcheck: TCheckBox
        Left = 10
        Top = 10
        Width = 53
        Height = 17
        Caption = 'events'
        TabOrder = 0
      end
      object netdebug: TCheckBox
        Left = 90
        Top = 55
        Width = 72
        Height = 17
        Caption = 'net debug'
        TabOrder = 1
      end
      object debcheck2: TCheckBox
        Left = 10
        Top = 56
        Width = 81
        Height = 17
        Caption = 'deb()'
        Checked = True
        State = cbChecked
        TabOrder = 2
      end
      object unkdeb: TCheckBox
        Left = 10
        Top = 33
        Width = 53
        Height = 17
        Caption = 'unknwn deb'
        TabOrder = 3
      end
      object Button6: TButton
        Left = 10
        Top = 84
        Width = 75
        Height = 25
        Caption = #1089#1090#1072#1090#1080#1089#1090#1080#1082#1072
        Enabled = False
        TabOrder = 4
        OnClick = Button6Click
      end
      object Button16: TButton
        Left = 10
        Top = 123
        Width = 75
        Height = 25
        Caption = 'heap'
        TabOrder = 5
        OnClick = Button16Click
      end
    end
    object TabSheet4: TTabSheet
      Caption = 'utils'
      ImageIndex = 3
      object find: TEdit
        Left = 16
        Top = 15
        Width = 102
        Height = 21
        TabOrder = 0
        Text = 'MZ'
      end
      object Button15: TButton
        Left = 124
        Top = 13
        Width = 36
        Height = 25
        Caption = 'find'
        TabOrder = 1
        OnClick = Button15Click
      end
    end
    object TabSheet5: TTabSheet
      Caption = 'RAM'
      ImageIndex = 4
      object Label11: TLabel
        Left = 10
        Top = 18
        Width = 23
        Height = 13
        Caption = #1084#1072#1082#1089
      end
      object Edit1: TEdit
        Left = 40
        Top = 16
        Width = 67
        Height = 17
        AutoSize = False
        NumbersOnly = True
        TabOrder = 0
        Text = '333 '#1052#1073
      end
    end
    object TabSheet6: TTabSheet
      Caption = 'FTP'
      ImageIndex = 5
      object diskcap: TLabel
        Left = 8
        Top = 32
        Width = 26
        Height = 13
        Caption = 'USER'
      end
      object Label10: TLabel
        Left = 8
        Top = 13
        Width = 10
        Height = 13
        Caption = 'IP'
      end
      object Label12: TLabel
        Left = 8
        Top = 51
        Width = 25
        Height = 13
        Caption = 'PASS'
      end
      object Label16: TLabel
        Left = 8
        Top = 111
        Width = 22
        Height = 13
        Caption = 'level'
      end
      object Label17: TLabel
        Left = -1
        Top = 70
        Width = 52
        Height = 13
        Caption = #1084#1072#1082#1089' '#1092#1072#1081#1083
      end
      object Label18: TLabel
        Left = 3
        Top = 82
        Width = 47
        Height = 23
        Caption = #1074#1089#1077#1075#1086' '#1084#1072#1082#1089' '#1084#1073
        WordWrap = True
      end
      object anonymous: TCheckBox
        Left = 183
        Top = 46
        Width = 97
        Height = 17
        Caption = 'anonymous'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object Button14: TButton
        Left = 183
        Top = 12
        Width = 121
        Height = 28
        Caption = #1087#1086#1076#1082#1083#1102#1095#1080#1090#1100
        TabOrder = 1
        WordWrap = True
        OnClick = Button14Click
      end
      object ftpip: TEdit
        Left = 56
        Top = 10
        Width = 121
        Height = 16
        AutoSize = False
        TabOrder = 2
        Text = 'ftp.rz.tu-bs.de'
      end
      object ftppass: TEdit
        Left = 56
        Top = 48
        Width = 121
        Height = 16
        AutoSize = False
        TabOrder = 3
        Text = 'xyu'
      end
      object ftpuser: TEdit
        Left = 56
        Top = 29
        Width = 121
        Height = 16
        AutoSize = False
        TabOrder = 4
        Text = 'xyu'
      end
      object dirlevel: TEdit
        Left = 56
        Top = 109
        Width = 24
        Height = 17
        AutoSize = False
        TabOrder = 5
        Text = '3'
      end
      object ftpmaxfilesize: TEdit
        Left = 56
        Top = 68
        Width = 121
        Height = 16
        AutoSize = False
        TabOrder = 6
        Text = '50000'
      end
      object ftpmaxds: TEdit
        Left = 56
        Top = 89
        Width = 121
        Height = 14
        AutoSize = False
        TabOrder = 7
        Text = '41111'
      end
    end
    object TabSheet7: TTabSheet
      Caption = #1044#1080#1089#1082
      ImageIndex = 6
      object Label14: TLabel
        Left = 25
        Top = 66
        Width = 68
        Height = 13
        Caption = #1054#1073#1088#1072#1079' '#1076#1080#1089#1082#1072':'
      end
      object Label15: TLabel
        Left = 41
        Top = 12
        Width = 52
        Height = 13
        Caption = #1053#1086#1089#1080#1090#1077#1083#1100':'
      end
      object ramonly: TCheckBox
        Left = 14
        Top = 189
        Width = 97
        Height = 17
        Hint = #1085#1077' '#1080#1089#1087#1086#1083#1100#1079#1086#1074#1072#1090#1100' '#1089#1077#1090#1100', '#1093#1088#1072#1085#1080#1090#1100' '#1074' '#1087#1072#1084#1103#1090#1080
        Caption = #1090#1086#1083#1100#1082#1086' RAM'
        Checked = True
        Ctl3D = True
        ParentCtl3D = False
        ParentShowHint = False
        ShowHint = True
        State = cbChecked
        TabOrder = 0
        OnExit = ramonlyExit
      end
      object Button1: TButton
        Left = 108
        Top = 7
        Width = 78
        Height = 25
        Caption = #1087#1086#1076#1082#1083#1102#1095#1080#1090#1100
        Enabled = False
        TabOrder = 1
        OnClick = Button1Click
      end
      object Button2: TButton
        Left = 199
        Top = 7
        Width = 78
        Height = 25
        Caption = #1086#1090#1082#1083#1102#1095#1080#1090#1100
        Enabled = False
        TabOrder = 2
        OnClick = Button2Click
      end
      object Button4: TButton
        Left = 108
        Top = 89
        Width = 78
        Height = 25
        Caption = #1089#1086#1093#1088#1072#1085#1080#1090#1100
        Enabled = False
        TabOrder = 3
        OnClick = Button4Click
      end
      object Button8: TButton
        Left = 199
        Top = 45
        Width = 78
        Height = 31
        Caption = #1079#1072#1075#1088#1091#1079#1080#1090#1100'  dump.dmp'
        TabOrder = 4
        WordWrap = True
        OnClick = Button8Click
      end
      object Button10: TButton
        Left = 199
        Top = 89
        Width = 78
        Height = 25
        Caption = #1079#1072#1075#1088#1091#1079#1080#1090#1100
        TabOrder = 5
        OnClick = Button10Click
      end
      object Button11: TButton
        Left = 108
        Top = 45
        Width = 78
        Height = 31
        Caption = #1089#1086#1093#1088#1072#1085#1080#1090#1100' dump.dmp'
        TabOrder = 6
        WordWrap = True
        OnClick = Button11Click
      end
      object Button3: TButton
        Left = 225
        Top = 299
        Width = 86
        Height = 25
        Caption = #1092#1086#1088#1084#1072#1090#1080#1088#1086#1074#1072#1090#1100
        Enabled = False
        TabOrder = 7
        OnClick = Button3Click
      end
    end
  end
  object pg2: TPageControl
    Left = 325
    Top = 2
    Width = 690
    Height = 560
    ActivePage = TabSheet1
    Anchors = []
    Style = tsFlatButtons
    TabOrder = 15
    object TabSheet1: TTabSheet
      Caption = 'log'
      object deblog: TListView
        Left = 0
        Top = -1
        Width = 679
        Height = 535
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clBtnHighlight
        Columns = <
          item
            Caption = #1083#1086#1075
            Width = 435
          end
          item
            Caption = 'thread'
            Width = 4
          end>
        DoubleBuffered = True
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Lucida Console'
        Font.Pitch = fpVariable
        Font.Style = []
        FlatScrollBars = True
        ReadOnly = True
        RowSelect = True
        ParentDoubleBuffered = False
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnCustomDrawSubItem = deblogCustomDrawSubItem
        OnDblClick = deblogDblClick
        OnKeyPress = deblogKeyPress
      end
    end
    object TabSheet8: TTabSheet
      Caption = 'stats'
      ImageIndex = 1
      object knotts: TChart
        Left = 0
        Top = 3
        Width = 679
        Height = 526
        Cursor = crDrag
        AllowPanning = pmNone
        BackWall.AutoHide = True
        Border.Color = 33023
        Border.Style = psDashDotDot
        Border.Visible = True
        Foot.Font.Name = 'Tahoma'
        Gradient.EndColor = 4162
        Gradient.MidColor = 10999983
        Gradient.StartColor = 12081670
        Gradient.SubGradient.EndColor = 33023
        Gradient.SubGradient.Visible = True
        Legend.Alignment = laBottom
        Legend.BevelWidth = 1
        Legend.Brush.Color = clGray
        Legend.Brush.Style = bsCross
        Legend.CheckBoxes = True
        Legend.Color = 16711808
        Legend.ColorWidth = 100
        Legend.Font.Charset = RUSSIAN_CHARSET
        Legend.Font.Color = 4210816
        Legend.Font.Name = 'Consolas'
        Legend.Font.Style = [fsBold]
        Legend.Font.Shadow.Color = 13355979
        Legend.Font.Shadow.HorizSize = 9
        Legend.Font.Shadow.Transparency = 11
        Legend.Font.Shadow.VertSize = 6
        Legend.Font.Shadow.Visible = False
        Legend.FontSeriesColor = True
        Legend.Frame.Color = 8421631
        Legend.Frame.Style = psDashDotDot
        Legend.Frame.EndStyle = esSquare
        Legend.Frame.Visible = False
        Legend.Gradient.Direction = gdLeftRight
        Legend.Gradient.EndColor = 16616349
        Legend.Gradient.MidColor = 9736694
        Legend.Gradient.StartColor = 2389129
        Legend.Shadow.Color = 329471
        Legend.Shadow.HorizSize = 2
        Legend.Shadow.SmoothBlur = 23
        Legend.Shadow.VertSize = 2
        Legend.Shadow.Visible = False
        Legend.Symbol.Shadow.Visible = False
        Legend.Symbol.Width = 100
        Legend.TextStyle = ltsXAndValue
        Legend.Title.Font.Name = 'Tahoma'
        Legend.Title.Transparent = False
        Legend.Title.Visible = False
        Legend.Title.Text.Strings = (
          'RAM MB / Disk MB')
        Legend.TopPos = 0
        Legend.Transparency = 92
        SubFoot.Font.Name = 'Tahoma'
        SubTitle.Font.Name = 'Tahoma'
        Title.Alignment = taLeftJustify
        Title.Brush.Color = clTeal
        Title.Brush.Style = bsDiagCross
        Title.Brush.Image.Data = {
          07544269746D61707E000000424D7E000000000000003E000000280000001000
          0000100000000100010000000000400000000000000000000000020000000200
          000000000000FFFFFF00AAAA000055550000AAAA000055550000AAAA00005555
          0000AAAA000055550000AAAA000055550000AAAA000055550000AAAA00005555
          0000AAAA000055550000}
        Title.Color = 4227072
        Title.CustomPosition = True
        Title.Font.Color = clRed
        Title.Font.Height = -12
        Title.Font.Name = 'Lucida Console'
        Title.Font.Gradient.EndColor = 4162
        Title.Font.Gradient.MidColor = 10999983
        Title.Font.Gradient.StartColor = 12081670
        Title.Font.OutLine.Color = 8421440
        Title.Font.OutLine.Style = psDot
        Title.Font.Shadow.Color = 11579568
        Title.Font.Shadow.HorizSize = 2
        Title.Font.Shadow.VertSize = 4
        Title.Font.Shadow.Visible = False
        Title.Frame.Width = 5
        Title.Gradient.EndColor = 2152289
        Title.Gradient.MidColor = 7548915
        Title.Gradient.StartColor = 10109259
        Title.Left = 45
        Title.Shadow.Visible = False
        Title.Text.Strings = (
          'fsizes')
        Title.Top = 0
        Title.Transparency = 14
        Title.VertMargin = 1
        BottomAxis.Axis.Style = psDot
        BottomAxis.Axis.EndStyle = esFlat
        BottomAxis.Axis.SmallDots = True
        BottomAxis.LabelsFont.Height = -8
        BottomAxis.LabelsFont.Name = 'Tahoma'
        DepthAxis.LabelsFont.Name = 'Tahoma'
        DepthTopAxis.LabelsFont.Name = 'Tahoma'
        LeftAxis.AxisValuesFormat = '0'
        LeftAxis.ExactDateTime = False
        LeftAxis.Grid.Visible = False
        LeftAxis.Increment = 5.000000000000000000
        LeftAxis.LabelsFont.Height = -9
        LeftAxis.LabelsFont.Name = 'Tahoma'
        LeftAxis.MaximumOffset = 34
        LeftAxis.MaximumRound = True
        LeftAxis.MinorTickCount = 0
        LeftAxis.Title.Angle = 0
        LeftAxis.Title.Caption = 'MB'
        LeftAxis.Title.Brush.Style = bsCross
        LeftAxis.Title.Color = clGray
        LeftAxis.Title.Transparency = 3
        LeftAxis.TitleSize = 6
        RightAxis.LabelsFont.Name = 'Tahoma'
        RightAxis.MaximumRound = True
        RightAxis.MinimumRound = True
        TopAxis.LabelsFont.Name = 'Tahoma'
        TopAxis.Visible = False
        View3D = False
        View3DOptions.HorizOffset = -4
        View3DOptions.Orthogonal = False
        View3DOptions.Zoom = 102
        View3DWalls = False
        Zoom.Allow = False
        Zoom.UpLeftZooms = True
        BevelOuter = bvNone
        Color = clWhite
        ParentShowHint = False
        ShowHint = False
        TabOrder = 0
        Visible = False
        object Series1: TLineSeries
          Marks.Arrow.Style = psDashDot
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Style = psDashDot
          Marks.Callout.Arrow.Visible = True
          Marks.Callout.Length = 14
          Marks.Brush.Color = clWhite
          Marks.DrawEvery = 9
          Marks.Font.Color = clNavy
          Marks.Font.Name = 'Tahoma'
          Marks.Frame.Style = psDashDot
          Marks.Frame.SmallDots = True
          Marks.Gradient.Visible = True
          Marks.Shadow.Color = 8553090
          Marks.Shadow.Visible = False
          Marks.Style = smsValue
          Marks.Visible = False
          PercentFormat = '#.#'
          SeriesColor = 12615680
          Title = #1088#1072#1079#1084#1077#1088' '#1092#1072#1081#1083#1086#1074
          ValueFormat = '#.#'
          Brush.Style = bsBDiagonal
          LineBrush = bsBDiagonal
          LineHeight = 5
          LinePen.Color = 10485760
          LinePen.Visible = False
          Pointer.Brush.Color = clWhite
          Pointer.Brush.Style = bsDiagCross
          Pointer.Brush.Image.Data = {
            07544269746D61707E000000424D7E000000000000003E000000280000001000
            0000100000000100010000000000400000000000000000000000020000000200
            000000000000FFFFFF00DDDD0000EEEE000077770000BBBB0000DDDD0000EEEE
            000077770000BBBB0000DDDD0000EEEE000077770000BBBB0000DDDD0000EEEE
            000077770000BBBB0000}
          Pointer.Gradient.EndColor = 12615680
          Pointer.InflateMargins = True
          Pointer.Pen.Style = psDot
          Pointer.Pen.Visible = False
          Pointer.Style = psHexagon
          Pointer.Visible = True
          TreatNulls = tnIgnore
          XValues.Name = 'X'
          XValues.Order = loNone
          YValues.Name = 'Y'
          YValues.Order = loNone
          Data = {
            001900000000000000008031400000000000C042400000000000205240000000
            0000005E4000000000000024400000000000C052400000000000002E40000000
            000040504000000000000059400000000000A05E400000000000404F40000000
            0000003E400000000000004E400000000000004E400000000000405F40000000
            0000E05A400000000000F069400000000000D071400000000000F06440000000
            0000D0714000000000003878400000000000987C400000000000BC8140000000
            0000407F400000000000007940}
        end
        object Series2: TLineSeries
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Arrow.Visible = True
          Marks.DrawEvery = 12
          Marks.Font.Color = clFuchsia
          Marks.Font.Name = 'Tahoma'
          Marks.Gradient.Visible = True
          Marks.Visible = False
          Shadow.HorizSize = 6
          Shadow.SmoothBlur = 25
          Shadow.Visible = False
          Title = #1087#1072#1084#1103#1090#1100
          Brush.Style = bsCross
          Brush.Image.Data = {
            07544269746D61707E000000424D7E000000000000003E000000280000001000
            0000100000000100010000000000400000000000000000000000020000000200
            000000000000FFFFFF0000000000AAAA000000000000AAAA000000000000AAAA
            000000000000AAAA000000000000AAAA000000000000AAAA000000000000AAAA
            000000000000AAAA0000}
          Dark3D = False
          LineBrush = bsCross
          LinePen.Color = clFuchsia
          LinePen.EndStyle = esFlat
          LinePen.SmallSpace = 2
          OutLine.Color = 16744703
          OutLine.Style = psDot
          Pointer.InflateMargins = True
          Pointer.Style = psRectangle
          Pointer.Visible = False
          Stairs = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          Data = {
            00190000000000000000AC624000000000008B634033333333330356409A9999
            9999F961400100000000E45A409A99999999B763400100000000A25C40676666
            6666965B40CDCCCCCCCCCC58406766666666164E403433333333C95040343333
            33337F594068666666669A4A409399999999992840CBCCCCCCCCC04B40313333
            3333734C40FFFFFFFFFFA157403233333333F35E403233333333F35E40999999
            9999B36440CCCCCCCCCC9061403233333333B95940CCCCCCCCCC485740CCCCCC
            CCCC505040E6FFFFFFFF7F0F40}
        end
        object Series3: TLineSeries
          Gradient.EndColor = 6308951
          Gradient.MidColor = 13529477
          Gradient.StartColor = 2514871
          Marks.Arrow.Color = 16711808
          Marks.Arrow.Style = psDashDotDot
          Marks.Arrow.EndStyle = esSquare
          Marks.Arrow.Visible = True
          Marks.Callout.Brush.Color = clBlack
          Marks.Callout.Style = psCircle
          Marks.Callout.Arrow.Color = 16711808
          Marks.Callout.Arrow.Style = psDashDotDot
          Marks.Callout.Arrow.EndStyle = esSquare
          Marks.Callout.Arrow.Visible = True
          Marks.Callout.ArrowHead = ahSolid
          Marks.BackColor = clBlue
          Marks.Brush.Color = clWhite
          Marks.Brush.Style = bsDiagCross
          Marks.Brush.Image.Data = {
            07544269746D617036100000424D361000000000000036000000280000002000
            0000200000000100200000000000001000000000000000000000000000000000
            0000EEC97300EECE7C00EDCE7D00EBCD7500EECE7D00EFD38800EED58C00EED4
            9000F1D79400F3D99700F4DA9800F2D79500EFD58F00F1D69100F2D69200F1D4
            9100F1D59200EFD89400EDD69200F1D89600F2D79900F3DA9C00F3D99E00F0D6
            9400EDD18200EECE7600EDCA7100EDCC7500EFCF7E00EECF7D00EECC7600EECB
            7100EDCE7D00F1D68F00EFD48D00ECCF7A00F0CD7900F1CF8100ECD18600F4DA
            9800F4DFAA00F7E5B800F6E1AD00F5DD9F00F7DB9F00F6DB9B00F5DC9B00F3DA
            9C00F5DBA100F7DEA600F6DFAB00F4E1AA00F8E2AB00F3DDA600F5DEAC00F5DF
            A900F3D89400EDD28500EED18400ECD28700F2D99600F0D89400F0D08300EDCE
            7A00EECE7E00F1D58F00EED58D00EDD18400EFCF7800F0CD7400EDCE7E00F0D6
            9300F7E3B000F7E6BC00F6E1B000F6DFA100F8DCA100F4D99800F1D89600F6DC
            9E00F8DDA400F7DDA800F6E4B700F6E6BB00F6E5B300F4E0AA00F5DFAD00F4DF
            A800F2D89600EED38800EFD58C00F2D89500F5DE9F00F0D89400EECE8100EECD
            7E00EFD08200F1D48A00F0D58B00EDD08200EBCD7600ECCA7100F1D38800F3DA
            9800F6DFA700F9E3AC00F9E0AA00F6DE9F00F2D79500F2D79400F4DA9C00F8DD
            A600F5DCA100F6DC9F00F8E5B300F7E7BC00F6E5B900F7E7B400F6E1AE00F7DE
            A500F2D79500F0D38500F0D38900F5DB9900F5DDA200F3D99700F0D08300EDCD
            7C00EFD28900EDCE8100EECF8100EECF7D00EDCE7400EECD7100EBD18200F1DA
            9700F7DFA800F5E0A800F7E0AC00F1DCA100F4DB9900EFD69200F5DDA600F7E0
            B000F3DDA300F4DA9900F4E0AA00F7E7B800F8E8B900F4E6B500F5E3B400F5DF
            A900F4DA9A00F1D38A00EFD08100EED28A00F1D89800F1D79400EED38900EFD2
            8800EED68D00EFD08000EDCE7C00F0CE7C00EDC96B00EFC86800F0CF7F00F3DA
            9700F6DDA700F3DEA500F5E1AD00F8E4B200F7DFAA00F5DDA700F8DFAA00F7DE
            A800F4DD9C00F3DA9800F6E2AB00F7E7B600F5E4B200F5E2AE00F5DEA900F5DB
            9F00F2D79500F2D58B00EFCF7A00EED28200F0D89200F3DB9700EDD79000EFD5
            9300EDD58A00EFCF7E00EFCD7500F0CA7400EBC55E00EBC56100EDCC7800EFD2
            8A00F6DA9D00F4DB9D00F6E0A800F9E6B800F5E5B800F6E6BB00F7E6B400F5E0
            AB00F6DEA600F8DFA600F6E0A800F7E2AB00F6DEA800F5DDA400F3DC9D00F1D7
            9600EFD59100EDD48A00F1CD7A00EDCD7A00F1D79200F4DA9D00F0D89500F1D6
            9400F0D07E00EEC76800EDC15B00EBC05700E7BE5200EAC15900EFC66800EDCD
            7800F0D28800F1D68F00F6DEA200F7E3B100F7E8BD00F7E7BF00F4E6B800F6E6
            B700F7E4B300F8E0AD00F4DC9C00F4DB9800F5DEA200F6E0A700F3DB9F00F6DB
            9C00F7DEA000F0D79400EFD18500EED38400F2D79200F7DDA300F5DC9D00F1D7
            9200EECA6D00E9BE4D00E5B63D00E7BB4700E8BC4F00E8BE5400ECC35E00F1C9
            6900EECA7100F1D08000F5D79400F5DFA600F7E5B900F5E6B900F5E9BF00F7E6
            BD00F4E0AB00F6DEA700F4DA9C00EFD89100F1D89600F5DC9B00F4DB9D00F4DC
            A400F7DDA100F2DA9700F1D89700F0D79400EFD79200F3DDA300F3DDA000EFD5
            9100F0C66200E8BA4700E5B83E00E5BC4700E7BE5000EEC15B00EDC46100EEC5
            6600EFCC7100EECD7800EFD18500F5DCA000F7E5B500F7E8BD00F8E8C100F8E6
            B600F6E0AA00F4DDA200F1D89600F1D69100F0D68F00EED38C00F2D79600F5DC
            A000F4DB9E00F6DC9F00F8DEA600F4DCA000F3DB9C00F7DFAC00F6DDA800EFD3
            8B00EFC56200E7BE5100E8BD4D00E8BD5000ECC15900EEC35E00EFC76500EEC8
            6700F0CD7200F2CF7B00F0CF8200F3D99500F7E3AF00F6E8BF00F6E5BC00F6E0
            A800F7DDA400F4DB9E00F1D79500F1D69100EFD58B00EFD58B00F1D69100F4D8
            9800F6DCA200F8E0A900F6DEAD00F5DEA700F4DDA100F7E0A800F5DDA400F1D4
            8700EDC76900EDC55E00E9BE5700E7BD5300E8BF5300EAC05700ECC45E00EDC8
            6900EECC7100EDCC7400EED07E00F5DC9900F7E0AB00F7E7BB00F5E2AF00F3D9
            9A00F5DB9D00F6DB9C00F3D99600F2D79300F1D89000F0D78E00F0D69000F1D6
            9500F6DEA700F5E7BE00F6E6BC00F7E2AF00F8DFA600F4DEA800F6DFA800F0D5
            8E00EDCF7A00EFC96B00F0C56700EAC25B00E7BF5100E9BD5200EBC25A00F1CB
            7000F0CE7400ECCE7500EFD38800F6DCA600F8E2AC00F8E4B300F5DD9F00F0D5
            9000F1D99800F2D89600F0D68F00F1D59400F2D79300F0D59000F2D69500F6DB
            9E00F4DEAA00F6E6B900F5E9BE00F5E1B200F8DFAA00F5E3B000F5E0AC00F0D8
            9900EED48600EFCF7C00EECB7300EEC86A00ECC25A00ECC35B00EEC76600EFCC
            7500F1D18500EFD48D00F4DA9800F7DEA500F6E1AC00F7E1AB00F4D79500EDD5
            8900EED68F00F1D48D00EFD18300F0D28600F1D38800F2D58D00F2D79A00F4DE
            AA00F7E2AD00F4E2B000F5E4B700F7E5B600F8E5B600F7E7BA00F7E0AC00F6DC
            9B00F3DA9A00F1D79700F1D69400F0D18700EFCE7900ECC96E00EEC96F00EFCE
            7C00F1D58D00F5DB9C00F6DDA100F3DFA400F8E0A900F7DEA600F2D69300EDD3
            8400F0D38400F0D07E00ECCB7200EDCD7400EECD7800EFCF7E00F2D48D00F3D9
            9C00F6DFA700F7E2AD00F8E2AD00F7E3B100FAE7BA00F6E7B900F7E0AC00FADE
            A100F5E0AD00F4E1B100F6E1AD00F4DA9B00F2D59100EDD18100EFCD7A00EECE
            7E00EFCE7F00F1D58E00F6DC9D00F2DDA400F5DDA000F4DA9A00EFD38D00EED0
            8000F0CE7600EEC97000ECC97000EECD7800F1CB7300EFC86900F0CE7900F0D5
            8B00F2D99A00F4DDA500F6DEA700F4DDA600F7E2AD00F8E4B100F7E1AD00F5DD
            A500F6E6B400F8E7BF00F7E6B900F4E0A700F5DBA100F2D89600EDD08300EECE
            7900F0CC7200ECCC7900EED28400EED79000F1D89600F2D69100F0D18700EFCC
            7900EEC76900EBC25F00EDC76700EFCB6E00EFC76D00EDC56200EDC86B00EECF
            8000EFD69000F3DA9B00F4DA9C00F3DA9C00F5DEA400F7DEA700F7DEA700F5DD
            A700F4E3B000F6E7C300F8E9C200F4E5B400F7E3B000F5DDA500EFD58D00EFCB
            7300EEC66500EFCC7200EFCE7C00F2D68E00EED38B00EFD28600F0CE7800ECC7
            6500E9C15500EABE5300EBC15800EDC25D00EFC56600F0C66300EDC56500F0CD
            7900F0D48800F0D48C00EFD38A00F0D69300F4DCA200F9DFA600F5DBA000F5DB
            A500F8E3B300F7EACA00FAECCB00F6E6BD00F7E5B400F5DDA600F2D79200F0D1
            8400EECA7200EECA6D00EFCB7100EECC7900F0D08000EFCF7A00EDC46300EABF
            5500E8BD4D00EABD5000E6BB4700E9BD4F00ECC15A00EFC56300EEC66900EECB
            7500EED08100EED08200EFD18400F2D79700F9E1AE00F5DDA300F4DA9B00F5DB
            9F00F8E7BE00F9F1D700FBEED300F7E4BB00F8E0AD00F8E2AE00F3DA9800EFD6
            9000ECD58E00EFD38400EECD7A00ECC86900EDC66900ECC66400E8BD4D00E7B9
            4500E9B94400E7B94400E7BB4200E9BD4B00E8BE5400EDC45F00EDC66600EECB
            7100EECD7A00EFD38800EFD79100F5DC9F00F7DEAA00F3DBA100F4D99B00F6DC
            A200F4E9BD00FBEED100F9EBC900F6E5B700F8E2AB00F9DFA600F6DD9F00F3D8
            9800F7DDA500F8DC9E00EED08300EEC66600EBC15900EBBE5200E8B63B00EAB6
            3200E8B53600E9B94200E7BB4B00E6BA4700EBBF5200EEC25C00EFC76700EECC
            7600F0D17E00F1D68E00EFD89300F4DA9B00F5DBA100F5DCA300F1DA9900F4DC
            A800F6E3B000F5E4B300F9E7BC00F8E6B800F6E3AF00F2D89D00F6DCA200F7DD
            A800F7E2AF00F3DA9D00EDCF8200EEC56500EBC15900E6BB4900E7B43300E8B2
            2F00E6B73D00E7BC4B00E7BB4B00E6B94600E9BD5000EDC35D00EFC96C00EFD0
            7E00F0D58C00F3D89400F2D68F00F0D38A00F2D58E00F6DC9D00F4DDA400F7DE
            A800F4DEA000F4DEA500F8E4B400F8E6B700F4E1AE00F6DFA800F6E0AB00F6E3
            B000F8E5B600F2DDA300EFD38900EFC76900EBC25900E6BA4700E7B43400E7B1
            2E00E7B94300E6BD5000E4BB4B00E6BC4700E7BD4F00ECC25B00EBC46500F0CF
            7B00F0D68F00F2D89600EFD18500EBCC7A00EECF7E00F0D79000F1DAA200F7DD
            A300F3D99900F5DFA900F7E3B500F8E6BB00F7E7BE00F8EBC700F6E8BE00F5E3
            B000F6E0AF00F3DEA200F0D48B00EFCA7000EAC35900E8BD4900E6B94000E8B6
            3D00E7BB4C00E4BC4C00E3BC4800E6B94100E7BB4600EABF5100ECC35E00EFC7
            6B00EECF7C00F0D38900F0CF8100EDCC7600EECB6F00EFCD7700F2D38800F1D6
            9300F5D89B00F7E2AF00F8E5BA00F7E7C100F9EDCF00F9EED200F8E8C000F8E2
            B100F7DFAA00F0D89700EFD48900F0CF7800EFC86A00EDC25C00E6BB4D00E9BE
            5000EAC15C00EABF5700E9BF5300E9BE5000E4BC4B00E5BD4F00EDC35D00F1C6
            6100F0CA6C00EFCB7500EFCB7200F0CD7400F1CC7200F0CB7000EECD7700EED2
            8600F1D68F00F5DFA900F9E9C100F5EBC500FAF0D400F8EFD100F7E8BD00F5E0
            AB00F2DA9F00EFD69300EFD48800EED07A00EDCE7900EFCB7500ECC35D00ECC2
            5B00EEC56100EEC66700EEC36100EDC55E00E7BF5200E6BD4F00E9C15700EAC2
            5800ECC35E00EDC46000EDC66800F0CD7400EFCD7700EEC76B00EFC66800F0CC
            7700EED07F00F1D89700F6E2AE00F9EAC500F9F1D400FAF1D400F8E8BE00F5DF
            A400F0D79500F1D58C00F0D08300EED07F00EDD07F00EECE7800EFCA7000EEC5
            6600F0C56300EEC66800F0C56400EDC46100EAC15700E7BD5100E9C15700E7BE
            5400E7BD5100E7BD5100E9C05800EBC46000EAC66300EBC15700E9BE5200EDC5
            6500ECC96E00F0D48900F3DA9B00F7E2B700F4EBC700FBF2D600F6E4B600F2DA
            9B00F2D79400F0D38D00EFD08300F0CE7800F0CF7E00EFCD7900F0CC7200F0CA
            6D00EAC05600E9BC4D00E8BE4F00EABF5700EEC45D00EAC05800EDC25800ECC1
            5700E7BC4E00E8BD5200E9BD5200E7BD4F00E6BD4E00E7B84300E5B94400E9C2
            5700EDC96C00F0D38700F2D89500F4DDA400F7E6BA00F9EDCB00F7E5B600F3D9
            9D00F1D69200F0D58B00F3D68A00EDCF7D00EFCF7E00ECD28700F0CD7C00EDC7
            6300E7BE4D00E8B94500E8BA4200E8BC4F00EBC35900EDC55D00EBC25B00EBC2
            5B00E9BF5900EDC35C00E7BD5400E7BB4B00E6BA4200E4B63900E6BA4300EAC3
            5900EDC86C00EFD28400F0D68D00EFD89000F8E1AF00F7E6BF00F7E3B300F5DC
            A200EFD48D00EDD18100EED18400ECD28600F0D79000F1D58B00EECA6F00EBC4
            5900E9BF5500E5BC4F00E5BA4600E7BC4D00E8C05400EDC55D00ECC46100F0C8
            6600F0C56900EEC46100EDC35E00EBBE4E00E7B73C00E7B94000E9BD5000EBC3
            5D00F1C86D00EECC7800ECCF7D00ECD28500F3DB9D00F7E0AC00F3DCA100F1D8
            9800EED18700EDCE7900EECE7A00F1D38600EFD68E00F1D28500EECA6F00EAC5
            6100ECC56300EBC35B00EBC05500E9C05300E8BE5500EAC46000ECCA6B00F0CC
            7200EEC56700EDC35B00EFC56200ECC05500E7B94600E9BD4C00ECC25A00EDC4
            6000F0C56700F1C86C00F2CE7400F0CF8200EED28400F2D48D00EFD58E00EED4
            8B00EFD08300F1D18000EED28400EFD48D00EED68A00EFD08400F0CF7D00EECF
            7700F0D08000EED07D00EECD7800EECD7100EDCC7200EFCE7B00EFD18300EDCD
            8200F2C96D00EEC35F00EDC35C00EAC15A00ECC25C00EEC36000EFC36200EDC3
            6200}
          Marks.Color = clBlue
          Marks.DrawEvery = 12
          Marks.Font.Color = clMaroon
          Marks.Frame.Style = psDash
          Marks.Gradient.Direction = gdFromCenter
          Marks.Gradient.Visible = True
          Marks.Shadow.Color = 8553090
          Marks.Shadow.HorizSize = 1
          Marks.Shadow.VertSize = 1
          Marks.Style = smsValue
          Marks.Visible = False
          SeriesColor = clGray
          Shadow.Visible = False
          Title = #1079#1072#1085#1103#1090#1086' '#1076#1080#1089#1082#1072
          Brush.Style = bsCross
          Brush.Image.Data = {
            07544269746D61707E000000424D7E000000000000003E000000280000001000
            0000100000000100010000000000400000000000000000000000020000000200
            000000000000FFFFFF0000000000AAAA000000000000AAAA000000000000AAAA
            000000000000AAAA000000000000AAAA000000000000AAAA000000000000AAAA
            000000000000AAAA0000}
          Dark3D = False
          LineBrush = bsCross
          LinePen.Color = clGray
          LinePen.Style = psDash
          LinePen.Width = 2
          OutLine.Color = clGray
          OutLine.Style = psDash
          Pointer.Brush.Style = bsCross
          Pointer.Brush.Image.Data = {
            07544269746D61707E000000424D7E000000000000003E000000280000001000
            0000100000000100010000000000400000000000000000000000020000000200
            000000000000FFFFFF00AAAA000055550000AAAA000055550000AAAA00005555
            0000AAAA000055550000AAAA000055550000AAAA000055550000AAAA00005555
            0000AAAA000055550000}
          Pointer.InflateMargins = True
          Pointer.Pen.Color = clBlue
          Pointer.Pen.Style = psDot
          Pointer.Style = psTriangle
          Pointer.Visible = False
          Stairs = True
          XValues.Name = 'X'
          XValues.Order = loAscending
          YValues.Name = 'Y'
          YValues.Order = loNone
          Data = {
            0019000000AD47E17A543E5240D6A3703D8AD25640703D0AD763955840C2F528
            5C6F0A64405B8FC2F568B25A40285C8FC2F5FA5F4051B81E85CBDB66408EC2F5
            281CBA6D405B8FC2F5889B6E4014AE47E10AF271408FC2F528DCAC7440C2F528
            5CDF977140EB51B81EF5C4714051B81E850BAA6F40D6A3703DAABB6A40CCCCCC
            CC5C8970401E85EB51181773403D0AD7A34086764000000000A04276408FC2F5
            28FC137940E17A14AE07077540703D0AD7D3A47540CCCCCCCC9C5779405B8FC2
            F5488E75405B8FC2F5D8C17740}
        end
      end
    end
    object tabftp: TTabSheet
      Caption = 'ftp log'
      ImageIndex = 2
      object Image2: TImage
        Left = 64
        Top = 509
        Width = 29
        Height = 20
        ParentShowHint = False
        Picture.Data = {
          0954506E67496D61676589504E470D0A1A0A0000000D49484452000000100000
          000A0806000000BDBEDE9C0000000774494D4507D7030612133767D430B80000
          00097048597300001EC200001EC2016ED0753E0000000467414D410000B18F0B
          FC6105000001764944415478DA85D23D4B42611407F0FFA3797D41942CCBAE17
          7C416CA9686BA8B5B5A54FE01028060EBD908194266AE66260B4545B43E25053
          440DF5055A1A1D24D4302BB1C8AE6FF7E90A6560713B709EE5707E70CE7948CC
          EDA2446780DED00FAD5A091915C0BFBFA35EAB41207278C3DB041241A2ABCBF4
          71661E5A96835929C0A810A097B540ABCFC85F9EC3E5DF14014A7BDABA284904
          83F4627C0EAF66271C8C00AB8870620E36AB28A50FE10D84FE007E10B21B8DD0
          B46D1677EC04AC4C1BA3627610B6FD067A7A005F60A367846FEC0BD88FC7E889
          691A37DC148CF2261C7D0D38E57570F517E8AE8EB1140A4B0347F108BD565B90
          E52631A0A03081C7305F81FAA900267B8BA544521AE83CB1753F658658E8342A
          9056038D6A05EDB72A16233BE47763CF26D63C0B74CC6EC108CB8251AA502A16
          90CFE550E779DC97CA4865CE241042486AC5474D363B38D6844EB5522EE3F1A1
          888FA6784A990C9E60ECFF11327BC9AEAC51C8BB807B2B2EF9893AF109B9E597
          0947E256010000000049454E44AE426082}
        ShowHint = True
        Stretch = True
        Transparent = True
        Visible = False
      end
      object Image1: TImage
        Left = -1
        Top = 509
        Width = 29
        Height = 20
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        ParentShowHint = False
        Picture.Data = {
          0954506E67496D61676589504E470D0A1A0A0000000D49484452000000100000
          000A0806000000BDBEDE9C0000000774494D4507D70306121406794BA6450000
          00097048597300001EC200001EC2016ED0753E0000000467414D410000B18F0B
          FC61050000015E4944415478DA85D1BF4B02611807F0EF7B97E70F44C9B2AED7
          034DC4968AB6865A5B5BFA0B1A0283A0A5A2DA2450B19682A2A5DA1A9286B688
          1AEA1F686974905043ADE412EDCE1FF7761668D9895F7886075E3E2FCFF310F4
          482CB4C888C305A7AB1F76AB191CD3A094CB502B156884070118FB794A881110
          DD5863F9D905D8A9048F5983DBA4C1C9D5C1E437A46FAF7B037BE130BB9998C7
          87278880A0C1A723925E833519B9C4A911D0ECDBD84134C212A37378A293F009
          0D8CE9D54468A3047675D20D68F7C7F118BB1067F0204DC3CDD710E8AB22C8AB
          90D47738EECE7F034621E42C1E61F7562F92D214064C0C22140C2B45585F3310
          928FE8F8F53FF07D89ED2D260C51386C16907A1555B9884649C64A64D768717F
          47D85C5E62E37E2F46288560B62097CD209D4A4155143CE70AE802B4E1C3F555
          268EFA2151114DB9582820FF92C5674D3F25C7C1F0749DB93CDA6F8D6833F12D
          20B413275F01398EEDFB5E599E0000000049454E44AE426082}
        Proportional = True
        ShowHint = True
        Stretch = True
        Transparent = True
        Visible = False
      end
      object imgpsvread: TImage
        Left = 37
        Top = 508
        Width = 21
        Height = 19
        ParentShowHint = False
        Picture.Data = {
          0954506E67496D61676589504E470D0A1A0A0000000D49484452000000100000
          000F0806000000ED734F2F0000000774494D4507D60111110631C71622230000
          00097048597300000B1200000B1201D2DD7EFC0000000467414D410000B18F0B
          FC6105000001A64944415478DAA591CB2F035114C6BF6999AA3415A5D498C423
          521BC4CE82AD050B1B7F01898454D28547540895A68A6E4824361E0BB1D05820
          418405125B1B4B0B11AF2A9A2165FA9839669A3491AA69C4777316F7DEEFFCF2
          9D1C0669F2F5761163B6A0C0520893D1001DC9102311443F3E20337A383C33CC
          773F930E981E1EA4A7964E98381EE50619D65C1905BA044878C1EDD101BA5C93
          DA00BFDB4D87F51D782BB7A3869551A94078A58AE30282811538C6A7B4010BD3
          5E0A54B5E2926B40252BA1562915C249EFA0ED6538C727B4014BB33EDAB435E3
          946F82551F474D4E0C767D147CF415E6E30D0C4C79B401ABB35E3A3156E08A6F
          44512EC10611A56218C6E73BB0571718F0CF6B03929B1875115BC2C19C9F0726
          11434C08437A17D0EF9DFBE1FFF130D2D74375D51528E338B0863C04EFEF707B
          7D8DA828E22618C2E2D68E7682C52127D9AAAAC1733690720F8742787AB8C767
          5C59A54E873EB72FFB08BBEB6B24041F93009224484A9172BA5D13D94748A9BD
          AD9DC646C7E0F17AB0B7BFF7ABEFFF00D598E9436D4E49856826383F3BCF0851
          1BBF83FE9440951A5DEB5FD51717C8AC165CB550610000000049454E44AE4260
          82}
        Proportional = True
        ShowHint = True
        Stretch = True
        Transparent = True
        Visible = False
      end
      object Image4: TImage
        Left = 99
        Top = 508
        Width = 24
        Height = 19
        ParentShowHint = False
        Picture.Data = {
          0954506E67496D61676589504E470D0A1A0A0000000D49484452000000100000
          001008060000001FF3FF610000028A4944415478DA8DD25B4814511807F0FF99
          CBEEAAEB6616A5522B5444B4E8225A81E42D2F2026B6BB965458EB6AA4747F30
          578B7C0822A5174348AC4C49C94C2D21B08B2BE516FA106E370DDFDC2E1495D1
          45DD1D7766A7334BF8229B9E879973663E7EDFFFCC1C82FF8C8234B58EDEFC7D
          4F85E9603524D88B1A9B2E3BD774A2CFE713C481FB4D39175A7E8F2C19C8DCCA
          93B2E25C57E181734665DDD16C1FB2560EA42D19B05BC34DC7AA1A7A5745AD0D
          AC3F4C4EA0B9A13AF362EB9FC14501A57BE9FEEDAE5D456546F7C729104210BB
          66053ADB2E0F959C1E495B14B05BB5E6A3A74EF684E922106BAC854EABC1D870
          0DBE7FF98CAB579A688AE9C1A040E616DA7D6F9CCBB227CF382BCC2136A19102
          2A8C3D3F0C35C7E356FB5D6749F5DBD4A0409535CC7CA46267CFEAE80878041F
          F4899D0160DC69819AE7F07EF21BAEB53CCCAA6B9D712C0094EEB6DDFA976653
          7C3CAB02660511FA240705388C0FA543C3B110E780CEAE51A7EDAC3B75015075
          30D45261DBDC1D15AD01CB2B808475C963D0857378FD7813343C03D1079A6206
          2D1D1359756DB38E79604712ED6E897865CA5B19C7A9014601BC7EA417FD4058
          28417F5B24342A028902A217B87DEFEBB3D2DAA99479A0B238A4B07C9FF64E54
          0C0B8EC6673860EA1783074FBC6068457E8E1A5A8D0CBF480181A6704BB8D13D
          9D5D7FD333403212395252C0BFC9CF20062E040180B040BF530553F94F7A0E80
          177DCB61582FC02F0112FD0E3E0FD0FB481E2E3BEF492639DBF89833A5F227C3
          06094A7C65FF0A3047E33A4638442E234832F8C0D267F23F404931FA8E932FB5
          431BD842FD714DD746BD54C872200C430166E1F196958B1F8114A20879C2CD5E
          B7377A0FCDFF8594044E4BF72B6309C34FAB9C2E714699FF05DE4AE39E9EFB5D
          440000000049454E44AE426082}
        ShowHint = True
        Stretch = True
        Transparent = True
      end
      object ftppb: TPaintBox
        Left = -2
        Top = 1
        Width = 683
        Height = 87
        OnMouseDown = ftplogMouseDown
      end
      object ftplog: TListView
        Left = -4
        Top = 88
        Width = 685
        Height = 418
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        Columns = <
          item
            Caption = 'ftp console'
          end>
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Lucida Console'
        Font.Style = []
        FlatScrollBars = True
        ReadOnly = True
        RowSelect = True
        ParentFont = False
        TabOrder = 0
        ViewStyle = vsReport
        OnClick = ftplogClick
        OnCustomDrawItem = ftplogCustomDrawItem
        OnDblClick = ftplogDblClick
      end
      object ftpent: TEdit
        Left = 132
        Top = 512
        Width = 550
        Height = 14
        AutoSize = False
        BevelInner = bvNone
        BevelOuter = bvNone
        BorderStyle = bsNone
        Color = clBtnFace
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlue
        Font.Height = -11
        Font.Name = 'Lucida Console'
        Font.Style = []
        ParentFont = False
        TabOrder = 1
        OnEnter = ftpentEnter
        OnKeyPress = ftpentKeyPress
      end
    end
  end
  object fatdeb: TCheckBox
    Left = 243
    Top = 97
    Width = 76
    Height = 14
    Caption = 'FAT debug'
    TabOrder = 16
  end
  object pop: TPopupMenu
    Left = 278
    Top = 48
  end
  object od: TOpenDialog
    Left = 224
    Top = 39
  end
end
