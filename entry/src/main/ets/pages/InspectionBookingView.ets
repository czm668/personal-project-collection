import { router } from "@kit.ArkUI";
import promptAction from '@ohos.promptAction';

@Entry
@Component
export struct InspectionBookingView {
  @State plateNumber: string = '';
  @State vin: string = '';
  @State showResults: boolean = false;
  @State availableTimes: Array<string> = [
    '2025-05-15 09:00-09:30',
    '2025-05-15 10:00-10:30',
    '2025-05-15 14:00-14:30',
    '2025-05-16 09:30-10:00',
    '2025-05-16 15:00-15:30'
  ];

  build() {
    Column() {
      // 返回按钮和标题
      Row() {
        Button('返回')
          .onClick(() => router.back())
          .margin({ left: 15 });
        Text('年检预约')
          .fontSize(20)
          .fontWeight(FontWeight.Bold);
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .justifyContent(FlexAlign.SpaceBetween);

      // 预约表单
      Column({ space: 20 }) {
        TextInput({ placeholder: '请输入车牌号', text: $$this.plateNumber })
          .width('90%')
          .height(50)
          .borderRadius(10)
          .backgroundColor('#F5F5F5')
          .margin({ top: 20 });
        TextInput({ placeholder: '请输入车辆识别代码', text: $$this.vin })
          .width('90%')
          .height(50)
          .borderRadius(10)
          .backgroundColor('#F5F5F5');
        Button('查询可预约时间')
          .width('90%')
          .height(50)
          .backgroundColor('#007DFF')
          .fontColor('#FFFFFF')
          .borderRadius(10)
          .onClick(() => this.queryAvailableTimes());
      }
      .width('100%')
      .alignItems(HorizontalAlign.Center);

      // 查询结果区域
      if (this.showResults) {
        Column() {
          Text('可预约时间')
            .fontSize(18)
            .fontWeight(FontWeight.Bold)
            .margin({ top: 20, bottom: 10 })
            .width('90%');

          // 可用时间列表
          ForEach(this.availableTimes, (time: string) => {
            Row() {
              Text(time)
                .fontSize(16)
                .margin({ left: 10 });
              Button('预约')
                .width(80)
                .height(35)
                .backgroundColor('#007DFF')
                .fontColor('#FFFFFF')
                .borderRadius(5)
                .onClick(() => this.bookTimeSlot(time));
            }
            .width('90%')
            .height(50)
            .backgroundColor('#FFFFFF')
            .borderRadius(10)
            .margin({ bottom: 10 })
            .justifyContent(FlexAlign.SpaceBetween)
            .alignItems(VerticalAlign.Center);
          });
        }
        .width('100%')
        .alignItems(HorizontalAlign.Center);
      }
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5');
  }

  // 查询可用时间
  queryAvailableTimes() {
    if (!this.plateNumber || !this.vin) {
      promptAction.showToast({ message: '请输入车牌号和车辆识别代码' });
      return;
    }

    // 查询过程
    this.showResults = true;
    promptAction.showToast({ message: '查询成功' });
  }

  // 预约时间
  bookTimeSlot(time: string) {
    promptAction.showToast({ message: `已预约 ${time}` });
  }
}