import { router } from "@kit.ArkUI";
import promptAction from '@ohos.promptAction';

@Entry
@Component
export struct ParkingPaymentView {
  build() {
    Column() {
      Row() {
        Button('返回')
          .onClick(() => router.back())
          .margin({ left: 15 });
        Text('停车缴费')
          .fontSize(20)
          .fontWeight(FontWeight.Bold);
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .justifyContent(FlexAlign.SpaceBetween);

      Column({ space: 20 }) {
        TextInput({ placeholder: '请输入停车编号', text: $$this.parkingNumber })
          .width('90%')
          .height(50)
          .borderRadius(10)
          .backgroundColor('#F5F5F5')
          .margin({ top: 20 });
        Button('查询缴费金额')
          .width('90%')
          .height(50)
          .backgroundColor('#007DFF')
          .fontColor('#FFFFFF')
          .borderRadius(10)
          .onClick(() => {
            promptAction.showToast({ message: '查询缴费金额功能待实现' });
          });
      }
      .width('100%')
      .alignItems(HorizontalAlign.Center);
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5');
  }
  @State parkingNumber: string = '';
}