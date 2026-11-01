import router from '@ohos.router';

@Entry
@Component
export struct ViolationView {
  build() {
    Column() {
      Row() {
        Button('返回')
          .onClick(() => {
            router.back();
          })
          .margin({ left: 15 })

        Text('违章查询')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .margin({ left: 100 })
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')

      Column() {
        Column() {
          Row() {
            Text('车牌号:')
              .fontSize(16)
              .margin({ left: 20, top: 10 })

            Text('粤A12345')
              .fontSize(16)
              .fontWeight(FontWeight.Bold)
              .margin({ left: 10, top: 10 })
          }

          Row() {
            Text('车辆类型:')
              .fontSize(16)
              .margin({ left: 20, top: 10 })

            Text('小型汽车')
              .fontSize(16)
              .margin({ left: 10, top: 10 })
          }

          Row() {
            Text('车架号后6位:')
              .fontSize(16)
              .margin({ left: 20, top: 10 })

            Text('123456')
              .fontSize(16)
              .margin({ left: 10, top: 10 })
          }
        }
        .width('90%')
        .height(150)
        .backgroundColor('#FFFFFF')
        .borderRadius(10)
        .margin({ top: 20, left: 20 })
        .shadow({
          radius: 5,
          color: '#CCCCCC'
        })

        Column() {
          Text('违章记录')
            .fontSize(18)
            .fontWeight(FontWeight.Bold)
            .margin({ left: 20, top: 20 })

          Column() {
            Row() {
              Text('2023-05-01')
                .fontSize(16)
                .margin({ left: 20, top: 10 })

              Text('闯红灯')
                .fontSize(16)
                .fontColor('#FF0000')
                .margin({ left: 20, top: 10 })
            }

            Row() {
              Text('地点:')
                .fontSize(16)
                .margin({ left: 20, top: 10 })

              Text('广州市天河区天河路')
                .fontSize(16)
                .margin({ left: 10, top: 10 })
            }

            Row() {
              Text('罚款: 200元')
                .fontSize(16)
                .margin({ left: 20, top: 10 })

              Text('扣分: 6分')
                .fontSize(16)
                .margin({ left: 30, top: 10 })
            }
          }
          .width('90%')
          .height(120)
          .backgroundColor('#FFFFFF')
          .borderRadius(10)
          .margin({ top: 10, left: 20 })

          Column() {
            Row() {
              Text('2023-04-15')
                .fontSize(16)
                .margin({ left: 20, top: 10 })

              Text('不按规定车道行驶')
                .fontSize(16)
                .fontColor('#FF0000')
                .margin({ left: 20, top: 10 })
            }

            Row() {
              Text('地点:')
                .fontSize(16)
                .margin({ left: 20, top: 10 })

              Text('广州市越秀区中山路')
                .fontSize(16)
                .margin({ left: 10, top: 10 })
            }

            Row() {
              Text('罚款: 100元')
                .fontSize(16)
                .margin({ left: 20, top: 10 })

              Text('扣分: 3分')
                .fontSize(16)
                .margin({ left: 30, top: 10 })
            }
          }
          .width('90%')
          .height(120)
          .backgroundColor('#FFFFFF')
          .borderRadius(10)
          .margin({ top: 10, left: 20 })
        }
      }
      .width('100%')
      .backgroundColor('#F5F5F5')
    }
    .width('100%')
    .height('100%')
  }
}

@Preview
struct ViolationViewPreview {
  build() {
    ViolationView()
  }
}