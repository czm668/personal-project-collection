import router from '@ohos.router';

interface ColorConstants {
  primary: string;
  background: string;
  cardBg: string;
  textPrimary: string;
  textSecondary: string;
  textTertiary: string;
  divider: string;
}
const COLORS: ColorConstants = {
  primary: '#007AFF',
  background: '#F8F9FD',
  cardBg: '#FFFFFF',
  textPrimary: '#1A1A1A',
  textSecondary: '#666666',
  textTertiary: '#999999',
  divider: '#EEEEEE'
};

interface SpacingConstants {
  xs: number;
  sm: number;
  md: number;
  lg: number;
  xl: number;
}
const SPACING: SpacingConstants = {
  xs: 4,
  sm: 8,
  md: 12,
  lg: 16,
  xl: 20
};

interface ServiceItem {
  id: number;
  name: string;
  icon: Resource;
  path?: string;
}

interface NoticeItem {
  title: string;
  content: string;
  date: string;
  path?: string;
}

interface ServiceCategory {
  title: string;
  items: ServiceItem[];
}

@Component
export struct ServiceView {
  @State itemScale: number = 1;
  @State serviceList: ServiceItem[] = [
    { id: 1, name: '违章查询', icon: $r('app.media.violation_icon'), path: 'pages/ViolationView' },
    { id: 2, name: '罚款缴纳', icon: $r('app.media.fine_payment_icon'), path: 'pages/FinePaymentView' },
    { id: 3, name: '驾驶证业务', icon: $r('app.media.driving_license_icon'), path: 'pages/DrivingLicensePage' },
    { id: 4, name: '机动车业务', icon: $r('app.media.vehicle_icon'), path: 'pages/VehicleBusinessView' },
    { id: 5, name: '停车缴费', icon: $r('app.media.service_parking'), path: 'pages/ParkingPaymentView' },
    { id: 6, name: '号牌补办', icon: $r('app.media.service_plate'), path: 'pages/NoticeDetailView' },
    { id: 7, name: '应急服务', icon: $r('app.media.service_emergency'), path: 'pages/EmergencyServiceView' },
    { id: 8, name: '交通路况', icon: $r('app.media.traffic_icon'), path: 'pages/TrafficConditionsView' },
    { id: 9, name: '电子驾照', icon: $r('app.media.service_digital'), path: 'pages/ElectronicLicenseView' },
    { id: 10, name: '考试预约', icon: $r('app.media.exam_booking_icon'), path: 'pages/ExamBookingView' },
    { id: 11, name: '年检预约', icon: $r('app.media.inspection_icon'), path: 'pages/InspectionBookingView' },
    { id: 12, name: '临牌申领', icon: $r('app.media.service_temp'), path: 'pages/TemporaryLicenseView' },
    { id: 13, name: '违法举报', icon: $r('app.media.service_report'), path: 'pages/ReportViolationView' },
    { id: 14, name: '充电地图', icon: $r('app.media.service_charging'), path: 'pages/ChargingMapView' },
    { id: 15, name: '车险办理', icon: $r('app.media.service_insurance'), path: 'pages/InsuranceServiceView' },
    { id: 16, name: '业务指南', icon: $r('app.media.ic_registration'), path: 'pages/BusinessGuideView' }

  ];
  @State notices: NoticeItem[] = [
    {
      title: '电子驾驶证申领流程优化',
      content: '即日起可通过App直接申领电子驾驶证',
      date: '2023-12-01',
      path: 'pages/NoticeList'
    },
    {
      title: '新增自助处理网点',
      content: '全市新增15处交通违法自助处理终端',
      date: '2023-11-28',
      path: 'pages/NoticeList'
    }
  ];
  @State serviceCategories: ServiceCategory[] = [
    {
      title: '驾驶证相关',
      items: this.serviceList.filter(item =>
      ['驾驶证业务', '电子驾照', '考试预约'].includes(item.name)
      )
    },
    {
      title: '机动车相关',
      items: this.serviceList.filter(item =>
      ['机动车业务', '年检预约', '临牌申领', '车险办理'].includes(item.name)
      )
    },
    {
      title: '交通违法处理',
      items: this.serviceList.filter(item =>
      ['违章查询', '罚款缴纳', '违法举报'].includes(item.name)
      )
    },
    {
      title: '出行服务',
      items: this.serviceList.filter(item =>
      ['停车缴费', '交通路况', '充电地图'].includes(item.name)
      )
    },
    {
      title: '其他专项',
      items: this.serviceList.filter(item =>
      ['号牌补办', '应急服务','业务指南'].includes(item.name)
      )
    }
  ];

  @Builder
  buildServiceSection(title: string, data: ServiceItem[]) {
    Column() {
      Row() {
        Text(title)
          .fontSize(20)
          .fontColor(COLORS.textPrimary)
          .fontWeight(FontWeight.Bold)
        Text('查看全部')
          .fontSize(14)
          .fontColor(COLORS.primary)
          .margin({ left: 'auto' })
          .onClick(() => {
          })
      }
      .padding({ bottom: SPACING.lg })
      GridRow({ columns: 4, gutter: SPACING.sm }) {
        ForEach(data, (item: ServiceItem) => {
          GridCol() {
            Column() {
              Image(item.icon)
                .width(48)
                .height(48)
                .objectFit(ImageFit.Contain)
                .margin({ bottom: SPACING.sm })
              Text(item.name)
                .fontSize(14)
                .fontColor(COLORS.textSecondary)
                .textAlign(TextAlign.Center)
                .maxLines(1)
                .textOverflow({ overflow: TextOverflow.Ellipsis })
            }
            .width('100%')
            .padding(SPACING.sm)
            .backgroundColor(COLORS.cardBg)
            .borderRadius(12)
            .shadow({ radius: 4, color: '#10000000', offsetY: 2 })
            .onClick(() => {
              animateTo({ duration: 100 }, () => {
                this.itemScale = 0.95;
                if (item.path) {
                  router.pushUrl({ url: item.path });
                }
              })
            })
          }
        })
      }
    }
    .width('100%')
    .padding({ left: SPACING.lg, right: SPACING.lg })
    .backgroundColor(COLORS.background)
  }

  @Builder
  buildNoticeItem(item: NoticeItem) {
    Column() {
      Row() {
        Text(item.title)
          .fontSize(16)
          .fontColor(COLORS.textPrimary)
          .fontWeight(FontWeight.Medium)
          .layoutWeight(1)
        Text(item.date)
          .fontSize(12)
          .fontColor(COLORS.textTertiary)
      }
      .padding({ bottom: SPACING.xs })
      Text(item.content)
        .fontSize(14)
        .fontColor(COLORS.textSecondary)
        .lineHeight(20)
        .maxLines(2)
        .textOverflow({ overflow: TextOverflow.Ellipsis })
    }
    .width('100%')
    .padding(SPACING.lg)
    .backgroundColor(COLORS.cardBg)
    .borderRadius(12)
    .shadow({ radius: 4, color: '#10000000', offsetY: 2 })
    .margin({ bottom: SPACING.lg })
    .onClick(() => {
      if (item.path) {
        router.pushUrl({ url: item.path });
      }
    })
  }

  build() {
    Column() {
      Row() {
        Image($r('app.media.back_icon'))
          .size({ width: 24, height: 24 })
          .margin({ left: SPACING.lg })
          .onClick(() => router.back())
        Text('交管服务')
          .fontSize(20)
          .fontColor(COLORS.textPrimary)
          .fontWeight(FontWeight.Bold)
          .layoutWeight(1)
          .textAlign(TextAlign.Center)
        Image($r('app.media.search_icon'))
          .size({ width: 24, height: 24 })
          .margin({ right: SPACING.lg })
      }
      .height(56)
      .backgroundColor(COLORS.cardBg)
      .shadow({ radius: 8, color: '#10000000', offsetY: 2 })
      Scroll() {
        Column() {
          ForEach(this.serviceCategories, (category: ServiceCategory) => {
            this.buildServiceSection(category.title, category.items)
          })
          Column() {
            Text('服务公告')
              .fontSize(20)
              .fontColor(COLORS.textPrimary)
              .fontWeight(FontWeight.Bold)
              .margin({ bottom: SPACING.lg })
            Column() {
              ForEach(this.notices, (item: NoticeItem) => {
                this.buildNoticeItem(item)
              })
            }
            .padding({ left: SPACING.lg, right: SPACING.lg })
            .backgroundColor(COLORS.background)
            .borderRadius(12)
            .margin({ left: SPACING.lg, right: SPACING.lg })
          }
          .padding({ top: SPACING.lg })
        }
      }
      .layoutWeight(1)
    }
    .width('100%')
    .height('100%')
    .backgroundColor(COLORS.background)
  }
}