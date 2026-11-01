import router from '@ohos.router';

@Entry
@Component
export  struct DrivingLicensePage {
  build() {
    Column() {
      Row() {
        Button('返回')
          .onClick(() => {
            router.back();
          })
          .margin({ left: 15 })

        Text('驾驶证业务')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .margin({ left: 80 })
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')

      Column() {
        Column() {
          Row() {
            Column() {
              Row() {
                Text('姓名:')
                  .fontSize(16)
                  .margin({ left: 20, top: 20 })

                Text('张三')
                  .fontSize(16)
                  .margin({ left: 10, top: 20 })
              }

              Row() {
                Text('驾驶证号:')
                  .fontSize(16)
                  .margin({ left: 20, top: 15 })

                Text('440101199001011234')
                  .fontSize(16)
                  .margin({ left: 10, top: 15 })
              }

              Row() {
                Text('准驾车型:')
                  .fontSize(16)
                  .margin({ left: 20, top: 10 })

                Text('C1')
                  .fontSize(16)
                  .margin({ left: 10, top: 10 })
              }

              Row() {
                Text('有效期:')
                  .fontSize(16)
                  .margin({ left: 20, top: 10 })

                Text('2018-05-01至2024-04-30')
                  .fontSize(16)
                  .margin({ left: 10, top: 10 })
              }
            }
          }
        }
        .width('90%')
        .height(200)
        .backgroundColor('#FFFFFF')
        .borderRadius(10)
        .margin({ top: 20, left: 1 })

        Column({ space: 15 }) {
          Row({ space: 20 }) {
            Column() {
              Image($r('app.media.driving'))
                .width(60)
                .height(60)

              Text('期满换证')
                .fontSize(14)
                .margin({ top: 5 })
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)
            .onClick(() => {
              router.pushUrl({
                url: 'pages/LicenseRenewalPage'
              });
            })

            Column() {
              Image($r('app.media.license_loss'))
                .width(60)
                .height(60)

              Text('遗失补证')
                .fontSize(14)
                .margin({ top: 5 })
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column() {
              Image($r('app.media.license_change'))
                .width(60)
                .height(60)

              Text('损毁换证')
                .fontSize(14)
                .margin({ top: 5 })
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column() {
              Image($r('app.media.license_info'))
                .width(60)
                .height(60)

              Text('信息变更')
                .fontSize(14)
                .margin({ top: 5 })
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)
          }

          Row({ space: 20 }) {
            Column() {
              Image($r('app.media.license_exam'))
                .width(60)
                .height(60)

              Text('提交体检')
                .fontSize(14)
                .margin({ top: 5 })
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column() {
              Image($r('app.media.license_points'))
                .width(60)
                .height(60)

              Text('满分学习')
                .fontSize(14)
                .margin({ top: 5 })
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)

            Column() {
              Image($r('app.media.license_other'))
                .width(60)
                .height(60)

              Text('其他业务')
                .fontSize(14)
                .margin({ top: 5 })
            }
            .width('20%')
            .alignItems(HorizontalAlign.Center)
          }
        }
        .width('80%')
        .margin({ top: 20, left: 20 })
      }
      .width('100%')
      .backgroundColor('#F5F5F5')
    }
    .width('100%')
    .height('100%')
  }
}

@Preview
struct DrivingLicensePagePreview {
  build() {
    DrivingLicensePage()
  }
}