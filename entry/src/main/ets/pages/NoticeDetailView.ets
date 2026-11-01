@Entry
@Component
export struct NoticeDetailView {
  build() {
    Column() {
      Text('号牌补办')
        .fontSize(20)
        .fontWeight(500)
        .margin(16)

      Column() {
        Row() {
          Text('申请时间：')
            .fontSize(14)
            .fontColor('#666')

          Text('2025-05-11 14:30')
            .fontSize(14)
            .fontColor('#333')
        }
        .margin({ bottom: 12 })

        Row() {
          Text('当前状态：')
            .fontSize(14)
            .fontColor('#666')

          Text('待审核')
            .fontSize(14)
            .fontColor('#1890ff')
            .fontWeight(500)
        }
        .margin({ bottom: 12 })

        Row() {
          Text('号牌信息：')
            .fontSize(14)
            .fontColor('#666')

          Text('京A12345（小型汽车）')
            .fontSize(14)
            .fontColor('#333')
        }
      }
      .backgroundColor('#fff')
      .padding(16)
      .margin({ left: 16, right: 16 })
      .borderRadius(12)
      .shadow({ radius: 4, color: '#0000001A' })

      Button('申请补办')
        .fontSize(16)
        .fontColor('#fff')
        .backgroundColor('#1890ff')
        .height(48)
        .margin({ top: 32, left: 16, right: 16 })
        .borderRadius(24)
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#f5f5f5')
  }
}