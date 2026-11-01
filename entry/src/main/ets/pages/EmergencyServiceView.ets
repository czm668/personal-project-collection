import { router } from "@kit.ArkUI";

@Entry
@Component
export struct EmergencyServiceView {
  build() {
    Column() {
      // 返回按钮和标题
      Row() {
        Button('返回')
          .onClick(() => router.back())
          .margin({ left: 15 });
        Text('应急服务')
          .fontSize(20)
          .fontWeight(FontWeight.Bold);
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .justifyContent(FlexAlign.SpaceBetween);

      // 应急服务
      Column({ space: 10 }) {
        Text('提供紧急救援、事故处理等应急服务：')
          .fontSize(16)
          .margin({ top: 20 });
        Text('紧急救援：拨打122')
          .fontSize(14)
          .margin({ top: 10 });
        Text('事故处理：保留现场，拨打报警电话')
          .fontSize(14)
          .margin({ top: 10 });
      }
      .width('100%')
      .margin({ top: 20 });
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5');
  }
}