interface NoticeItem {
  title: string;
  content: string;
  date: string;
  path: string;
  read?: boolean;
  noticeType?: string;
  hasAttachment?: boolean;
}

interface TypeColorMap {
  '服务升级': string;
  '政策通知': string;
  '设施新增': string;
  '临时通知': string;
  '办事指南': string;
  '设施调整': string;
}

@Entry
@Component
export struct NoticeList {
  @State noticeList: NoticeItem[] = [
    {
      title: '电子驾驶证申领流程优化',
      content: '即日起可通过"交管12123"App直接申领电子驾驶证，支持人脸识别快速认证。申领成功后可在全国范围内使用，无需携带实体证件。',
      date: '2023-12-01',
      path: 'pages/notice/NoticeDetail',
      read: false,
      noticeType: '服务升级',
      hasAttachment: false
    },
    {
      title: '全市新增15处交通违法自助处理终端',
      content: '24小时自助终端覆盖万达商圈、市民服务中心等15个点位，支持处理罚款200元以下非现场违法。需携带驾驶证、行驶证原件及本人身份证。',
      date: '2023-11-28',
      path: 'pages/notice/NoticeDetail',
      read: true,
      noticeType: '设施新增',
      hasAttachment: true
    },
    {
      title: '驾驶证期满换证线上办理指南',
      content: '线上换证流程：登录App→提交体检报告→上传一寸照片→选择邮寄地址。审核通过后3个工作日内寄出，工本费10元。',
      date: '2023-11-25',
      path: 'pages/notice/NoticeDetail',
      read: false,
      noticeType: '办事指南',
      hasAttachment: false
    },
    {
      title: '冬季停车优惠政策上线',
      content: '2023年12月1日 - 2024年2月28日期间，公共停车场夜间时段（20:00 - 8:00）停车费5折，新能源车辆额外享受8折，需扫码支付生效。',
      date: '2023-11-20',
      path: 'pages/notice/NoticeDetail',
      read: true,
      noticeType: '政策通知',
      hasAttachment: false
    },
    {
      title: '周末限行政策临时调整',
      content: '因国际马拉松赛事，11月25 - 26日市区限行尾号调整：周六3/8，周日4/9。限行时间7:00 - 20:00，绕行路线请查看附件地图。',
      date: '2023-11-22',
      path: 'pages/notice/NoticeDetail',
      read: false,
      noticeType: '临时通知',
      hasAttachment: true
    },
    {
      title: '交通违法线上申诉功能升级',
      content: '新增智能识别系统自动匹配违法场景，申诉材料支持行驶证、行车记录仪视频上传，处理周期缩短至3个工作日。',
      date: '2023-11-18',
      path: 'pages/notice/NoticeDetail',
      read: true,
      noticeType: '服务升级',
      hasAttachment: false
    },
    {
      title: '春运期间高速公路免费通行安排',
      content: '2024年2月9日0时 - 2月17日24时，7座及以下小型客车免费通行全国高速公路。建议错峰出行，提前规划充电/休息站点。',
      date: '2023-12-10',
      path: 'pages/notice/NoticeDetail',
      read: false,
      noticeType: '政策通知',
      hasAttachment: false
    },
    {
      title: '新能源汽车专用停车位新增200个',
      content: '市区核心商圈、写字楼周边新增200个新能源专用车位，支持快充/慢充设备。停车前需打开车牌识别系统完成认证。',
      date: '2023-12-05',
      path: 'pages/notice/NoticeDetail',
      read: true,
      noticeType: '设施新增',
      hasAttachment: true
    },
    {
      title: '驾驶证审验教育线上课程更新',
      content: '审验教育平台新增防御性驾驶、恶劣天气应对等课程，累计学习3小时可在线申请审验。支持手机/电脑端同步学习进度。',
      date: '2023-12-03',
      path: 'pages/notice/NoticeDetail',
      read: false,
      noticeType: '服务升级',
      hasAttachment: false
    },
    {
      title: '交通标志标线优化调整公告',
      content: '即日起对市区10处复杂路口标志标线进行优化，包括新增左转待转区、调整车道导向箭头。具体位置见附件示意图。',
      date: '2023-12-02',
      path: 'pages/notice/NoticeDetail',
      read: true,
      noticeType: '设施调整',
      hasAttachment: true
    },
    {
      title: '元旦假期交通管制通知',
      content: '12月31日18:00 - 24:00，市中心区域部分道路实施临时交通管制，禁止机动车停放。管制路段详见App内实时地图标注。',
      date: '2023-11-30',
      path: 'pages/notice/NoticeDetail',
      read: false,
      noticeType: '临时通知',
      hasAttachment: false
    },
    {
      title: '货车禁区通行许可线上申请开通',
      content: '市区货车禁区通行许可实现全程网办，登录"交管12123"提交行驶证、运输合同等材料，3个工作日内完成审核。',
      date: '2023-11-29',
      path: 'pages/notice/NoticeDetail',
      read: true,
      noticeType: '办事指南',
      hasAttachment: false
    }
  ];

  private typeColorMap: Record<string, string> = {
    '服务升级': '#409EFF',
    '政策通知': '#2FBA72',
    '设施新增': '#FFB820',
    '临时通知': '#FF5722',
    '办事指南': '#909399',
    '设施调整': '#67C23A'
  };

  build() {
    Column() {
      Row() {
        Text('服务公告').fontSize(24).fontWeight(FontWeight.Bold);
        Text(`(${this.noticeList.filter(n => !n.read).length})未读`)
          .fontSize(14)
          .fontColor('#666')
          .margin({ left: 8 });
      }
      .margin({ top: 24, bottom: 16 })
      .width('100%');

      List() {
        ForEach(this.noticeList, (item: NoticeItem) => {
          ListItem() {
            Navigator({ target: item.path }) {
              Column({ space: 8 }) {
                Row({ space: 4 }) {
                  Text(item.title).fontSize(16).fontWeight(FontWeight.Medium);
                  if (item.noticeType) {
                    Text(item.noticeType)
                      .fontSize(12)
                      .padding(4)
                      .backgroundColor(this.typeColorMap[item.noticeType] || '#E0E0E0')
                      .fontColor('#FFFFFF')
                      .borderRadius(4);
                  }
                }
                Text(item.content)
                  .fontSize(14)
                  .fontColor('#333')
                  .maxLines(2)
                  .textOverflow({ overflow: TextOverflow.Ellipsis });
                Row() {
                  Text(item.date).fontSize(12).fontColor('#666');
                }
                .margin({ top: -4 })
                .width('100%');
              }
            }
            .padding(16)
            .backgroundColor(item.read ? '#F8F8F8' : '#FFFFFF')
            .border({ width: 1, color: item.read ? 'transparent' : '#E8E8E8' })
            .borderRadius(12)
            .shadow({
              radius: 3,
              offsetX: 3,
              offsetY: 6,
              color: '#F5F5F5'
            });
          }
        });
      }
      .width('100%')
      .flexGrow(1);
    }
    .backgroundColor('#F0F0F0')
    .height('100%')
    .padding({ left: 16, right: 16 });
  }
}