import { router } from "@kit.ArkUI";
import promptAction from '@ohos.promptAction';

@Entry
@Component
export struct ReportViolationView {
  @State showHint1: boolean = true;
  @State showHint2: boolean = true;
  @State inputText1: string = '';
  @State inputText2: string = '';

  build() {
    Column() {
      Row() {
        Button('返回')
          .onClick(() => router.back())
          .margin({ left: 15 });
        Text('违法举报')
          .fontSize(20)
          .fontWeight(FontWeight.Bold);
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .justifyContent(FlexAlign.SpaceBetween);

      Column() {
        Text('违法类型')
          .fontSize(14)
          .fontColor('#666')
          .margin({ top: 12, bottom: 4 });

        if (this.showHint1) {
          Text('请选择违法类型')
            .fontSize(14)
            .fontColor('#999')
            .backgroundColor('#F5F5F5')
            .padding(12)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .onClick(() => {
              this.showHint1 = false;
            });
        } else {
          TextInput()
            .fontSize(14)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .padding(12)
            .onChange((value) => {
              this.inputText1 = value;
            })
            .onBlur(() => {
              if (!this.inputText1) {
                this.showHint1 = true;
              }
            });
        }
      }
      .margin({ left: 16, right: 16 });

      Column() {
        Text('上传违法证据')
          .fontSize(14)
          .fontColor('#666')
          .margin({ top: 24, left: 16, bottom: 8 });

        Row() {
          Image('icon_camera.png')
            .width(80)
            .height(80)
            .border({ width: 1, color: '#eee' })
            .borderRadius(8)
            .margin(16);
        }
      }

      Column() {
        Text('举报详情')
          .fontSize(14)
          .fontColor('#666')
          .margin({ top: 24, bottom: 4, left: 16 });

        if (this.showHint2) {
          Text('请详细描述违法经过（最多200字）')
            .fontSize(14)
            .fontColor('#999')
            .backgroundColor('#F5F5F5')
            .padding(12)
            .border({ width: 1, color: '#eee' })
            .borderRadius(12)
            .onClick(() => {
              this.showHint2 = false;
            });
        } else {
          TextInput()
            .fontSize(14)
            .border({ width: 1, color: '#eee' })
            .borderRadius(12)
            .padding(12)
            .height(120)
            .maxLines(3)
            .onChange((value) => {
              this.inputText2 = value;
            })
            .onBlur(() => {
              if (!this.inputText2) {
                this.showHint2 = true;
              }
            });
        }
      }
      .margin({ left: 16, right: 16 });

      Button('提交举报')
        .fontSize(16)
        .fontColor('#fff')
        .backgroundColor('#1890ff')
        .height(48)
        .margin({ top: 32, left: 16, right: 16 })
        .borderRadius(24)
        .onClick(() => {
          if (!this.inputText1 ||!this.inputText2) {
            promptAction.showToast({ message: '请填写违法类型和举报详情' });
            return;
          }
          promptAction.showToast({ message: '举报提交成功' });
        });
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#f5f5f5');
  }
}