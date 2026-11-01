import { router } from "@kit.ArkUI";
import promptAction from '@ohos.promptAction';

@Entry
@Component
export struct MoreServicesView {
  build() {
    Column() {
      // 返回按钮和标题
      Row() {
        Button('返回')
          .onClick(() => router.back())
          .margin({ left: 15 });
        Text('更多服务')
          .fontSize(20)
          .fontWeight(FontWeight.Bold);
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .justifyContent(FlexAlign.SpaceBetween);

      // 服务列表
      List({ space: 10 }) {
        ListItem() {
          Row() {
            Text('业务一')
              .fontSize(16)
              .width('80%');
            Image($r('app.media.arrow_right'))
              .width(20)
              .height(20);
          }
          .onClick(() => {
            promptAction.showToast({ message: '业务一功能待实现' });
          });
        }
        ListItem() {
          Row() {
            Text('业务二')
              .fontSize(16)
              .width('80%');
            Image($r('app.media.arrow_right'))
              .width(20)
              .height(20);
          }
          .onClick(() => {
            promptAction.showToast({ message: '业务二功能待实现' });
          });
        }
        ListItem() {
          Row() {
            Text('业务三')
              .fontSize(16)
              .width('80%');
            Image($r('app.media.arrow_right'))
              .width(20)
              .height(20);
          }
          .onClick(() => {
            promptAction.showToast({ message: '业务三功能待实现' });
          });
        }
      }
      .width('100%')
      .margin({ top: 20 });
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5');
  }
}