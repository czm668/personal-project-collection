import router from '@ohos.router';

@Entry
@Component
export struct ElectronicCertificateView {
  build() {
    Column() {
      // 顶部导航栏
      Row({ space: 5 }) {
        Image($r('app.media.back_icon'))
          .width(24)
          .height(24)
          .margin({ left: 15 })
          .onClick(() => router.back())

        Text('电子凭证')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')

      // 电子凭证内容区域
      Column() {
        Text('这是电子凭证页面')
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
struct ElectronicCertificateViewPreview {
  build() {
    ElectronicCertificateView()
  }
}