import { router } from "@kit.ArkUI";

interface NoticeItem {
  id: number;
  title: string;
  date: string;
}

@Component
export struct HomeView {
  build() {
    Scroll() {
      Column() {
        Row({ space: 5 }) {
          Image($r('app.media.back_icon'))
            .width(24)
            .height(24)
            .margin({ left: 15 })

          Text('交管12123')
            .fontSize(20)
            .fontWeight(FontWeight.Bold)

          Image($r('app.media.search_icon'))
            .onClick(() => {
              router.pushUrl({
                url: 'pages/SearchView'
              });
            })
            .width(24)
            .height(24)
            .margin({ left: 180 })

          Image($r('app.media.message_icon'))
            .width(24)
            .height(24)
            .margin({ left: 20 })
        }
        .width('100%')
        .height(60)
        .backgroundColor('#FFFFFF')

        Stack() {
          Image($r('app.media.user_bg'))
            .width('100%')
            .height(170)

          Row({ space: 20 }) {
            Image($r('app.media.user_avatar'))
              .width(70)
              .height(100)
              .borderRadius(35)
              .margin({ left: 20 })

            Column({ space: 5 }) {
              Text('持证人：张三')
                .fontSize(20)
                .fontWeight(FontWeight.Bold)
                .fontColor(Color.Black)

              Text('驾驶证: ************1234')
                .fontSize(14)
                .fontColor(Color.Black)

              Text('行驶证: ************5678')
                .fontSize(14)
                .fontColor(Color.Black)
            }
            .margin({ top: 15 })

            Button('绑定证件')
              .width(90)
              .height(30)
              .backgroundColor('#FFFFFF')
              .fontColor('#007DFF')
              .fontSize(14)
              .borderRadius(15)
              .margin({ left: 1, top: 70 })
          }
          .width('100%')
          .margin({ top: 20 })
        }
        .width('100%')
        .height(150)

        Column({ space: 10 }) {
          Row({ space: 10 }) {
            Column({ space: 5 }) {
              Image($r('app.media.violation_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/ViolationView'
                  });
                })
                .width(48)
                .height(48)
              Text('违章查询')
                .fontSize(14)

            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column({ space: 5 }) {
              Image($r('app.media.fine_payment_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/FinePaymentView'
                  });
                })
                .width(48)
                .height(48)

              Text('罚款缴纳')
                .fontSize(14)

            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column({ space: 5 }) {
              Image($r('app.media.driving_license_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/DrivingLicensePage'
                  });
                })
                .width(48)
                .height(48)

              Text('驾驶证业务')
                .fontSize(14)
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column({ space: 5 }) {
              Image($r('app.media.vehicle_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/VehicleBusinessView'
                  });
                })
                .width(48)
                .height(48)
              Text('机动车业务')
                .fontSize(14)

            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)
          }

          Row({ space: 10 }) {
            Column({ space: 5 }) {
              Image($r('app.media.inspection_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/InspectionBookingView'
                  });
                })
                .width(48)
                .height(48)
              Text('年检预约')
                .fontSize(14)

            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column({ space: 5 }) {
              Image($r('app.media.traffic_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/TrafficConditionsView'
                  });
                })
                .width(48)
                .height(48)
              Text('交通路况')
                .fontSize(14)

            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column({ space: 5 }) {
              Image($r('app.media.exam_booking_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/ExamBookingView'
                  });
                })
                .width(48)
                .height(48)
              Text('考试预约')
                .fontSize(14)

            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column({ space: 5 }) {
              Image($r('app.media.more_services_icon'))
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/MoreServicesView'
                  });
                })
                .width(48)
                .height(48)
              Text('更多服务')
                .fontSize(14)

            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)
          }
        }
        .width('100%')
        .margin({ top: 20, left: 15, right: 15 })

        Column() {
          Row() {
            Image($r('app.media.services_icon'))
              .width(20)
              .height(20)
              .margin({ left: 15 })

            Text('常用服务')
              .fontSize(16)
              .fontWeight(FontWeight.Bold)
              .margin({ left: 10 })

            Text('全部')
              .fontSize(14)
              .fontColor('#007DFF')
              .margin({ left: 250 })
          }
          .width('100%')
          .height(40)

          Scroll() {
            Row({ space: 15 }) {
              Column({ space: 5 }) {
                Image($r('app.media.service_emergency'))
                  .onClick(() => {
                    router.pushUrl({
                      url: 'pages/EmergencyServiceView'
                    });
                  })
                  .width(48)
                  .height(48)
                Text('应急服务')
                  .fontSize(14)

              }

              Column({ space: 5 }) {
                Image($r('app.media.service_parking'))
                  .onClick(() => {
                    router.pushUrl({
                      url: 'pages/ParkingPaymentView'
                    });
                  })
                  .width(48)
                  .height(48)
                Text('停车缴费')
                  .fontSize(14)

              }

              Column({ space: 5 }) {
                Image($r('app.media.service_insurance'))
                  .onClick(() => {
                    router.pushUrl({
                      url: 'pages/InsuranceServiceView'
                    });
                  })
                  .width(48)
                  .height(48)
                Text('车险办理')
                  .fontSize(14)

              }

              Column({ space: 5 }) {
                Image($r('app.media.service_charging'))
                  .onClick(() => {
                    router.pushUrl({
                      url: 'pages/ChargingMapView'
                    });
                  })
              }
                  .width(48)
                  .height(48)
                Text('充电地图')
                  .fontSize(14)

            }
            .padding({ left: 15, right: 15 })
          }
          .height(100)
          .scrollable(ScrollDirection.Horizontal)
        }
        .width('100%')
        .backgroundColor('#FFFFFF')
        .margin({ top: 20 })

        Column() {
          Row() {
            Image($r('app.media.announce_icon'))
              .width(20)
              .height(20)
              .margin({ left: 15 })

            Text('最新公示')
              .fontSize(16)
              .fontWeight(FontWeight.Bold)
              .margin({ left: 10 })

            Text('历史公示')
              .fontSize(14)
              .fontColor('#007DFF')
              .margin({ left: 240 })
          }
          .width('100%')
          .height(40)

          Column({ space: 12 }) {
            Column() {
              Row() {
                Text('新规发布')
                  .fontSize(12)
                  .fontColor('#FFFFFF')
                  .backgroundColor('#FF6A00')
                  .borderRadius(3)
                  .padding(4)

                Text('2023-12-01 实施')
                  .fontSize(12)
                  .fontColor('#999999')
                  .margin({ left: 8 })
              }

              Text('《道路交通安全违法行为记分管理办法》修订版')
                .fontSize(14)
                .margin({ top: 6 })
                .maxLines(1)
                .textOverflow({ overflow: TextOverflow.Ellipsis })
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/AnnouncementDetailView',
                    params: { id: 1 }
                  });
                })

            }
            .width('90%')
            .padding(10)
            .backgroundColor('#FFFFFF')
            .borderRadius(8)
            .shadow({
              radius: 4,
              color: '#20000000',
              offsetX: 1,
              offsetY: 1
            })

            Column() {
              Row() {
                Text('政策解读')
                  .fontSize(12)
                  .fontColor('#FFFFFF')
                  .backgroundColor('#007DFF')
                  .borderRadius(3)
                  .padding(4)

                Text('2023-11-25 发布')
                  .fontSize(12)
                  .fontColor('#999999')
                  .margin({ left: 8 })
              }

              Text('新能源汽车专用号牌申领流程优化说明')
                .fontSize(14)
                .margin({ top: 6 })
                .maxLines(1)
                .textOverflow({ overflow: TextOverflow.Ellipsis })
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/AnnouncementDetailView',
                    params: { id: 2 }
                  });
                })
            }
            .width('90%')
            .padding(10)
            .backgroundColor('#FFFFFF')
            .borderRadius(8)
            .shadow({
              radius: 4,
              color: '#20000000',
              offsetX: 1,
              offsetY: 1
            })
          }
          .width('100%')
          .alignItems(HorizontalAlign.Center)
        }
        .width('100%')
        .backgroundColor('#F5F5F5')
        .margin({ top: 20 })

        Column() {
          Row() {
            Image($r('app.media.notice_icon'))
              .width(20)
              .height(20)
              .margin({ left: 15 })

            Text('通知公告')
              .fontSize(16)
              .fontWeight(FontWeight.Bold)
              .margin({ left: 10 })

            Text('更多')
              .fontSize(14)
              .fontColor('#007DFF')
              .margin({ left: 250 })
              .onClick(() => {
                router.pushUrl({
                  url: 'pages/NoticeListView'
                });
              })
          }
          .width('100%')
          .height(40)

          List() {
            ForEach([
              { id: 1, title: '关于开展交通安全宣传活动的通知', date: '2023-05-10' },
              { id: 2, title: '某路段施工交通管制公告', date: '2023-05-09' },
              { id: 3, title: '驾驶证期满换证业务优化通知', date: '2023-05-08' }
            ] as NoticeItem[], (item: NoticeItem) => {
              ListItem() {
                Row() {
                  Text(item.title)
                    .fontSize(14)
                    .width('70%')
                    .onClick(() => {
                      router.pushUrl({
                        url: 'pages/NoticeView',
                        params: { id: item.id }
                      });
                    })

                  Text(item.date)
                    .fontSize(12)
                    .fontColor('#999999')
                    .margin({ left: 30 })
                }
                .width('100%')
                .height(40)
              }
            })
          }
          .width('100%')
          .height(120)
        }
        .width('100%')
        .backgroundColor('#FFFFFF')
        .margin({ top: 20 })
      }
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5')
  }
}

@Preview
struct HomeViewPreview {
  build() {
    HomeView()
  }
}