import router from '@ohos.router';

@Entry
@Component
export struct ProgressQueryView {
  build() {
    Column() {
      Row({ space: 5 }) {
        Image($r('app.media.back_icon'))
          .width(24)
          .height(24)
          .margin({ left: 15 })
          .onClick(() => router.back())

        Text('进度查询')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')

      Column() {
        Text('这是进度查询页面')
          .fontSize(16)
          .margin({ top: 20 })
      }
      .width('100%')
      .padding(20)
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5')
  }
}

@Preview
struct ProgressQueryViewPreview {
  build() {
    ProgressQueryView()
  }
}