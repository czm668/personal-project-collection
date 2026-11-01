import { router } from "@kit.ArkUI";

interface FunctionItem {
  icon: string;
  name: string;
  path?: string;
}

@Component
export struct UserView {
  private readonly ITEM_HEIGHT: number = 60;
  private readonly AVATAR_SIZE: number = 88;

  @State functionItems: FunctionItem[] = [];

  aboutToAppear() {
    this.initFunctionItems();
  }

  initFunctionItems() {
    this.functionItems = [
      { icon: 'icon_todo', name: '我的信息', path: 'pages/UserInfo' },
      { icon: 'icon_course', name: '账号与安全', path: 'pages/AccountSecurity' },
      { icon: 'icon_cloud_disk', name: '邮寄地址', path: 'pages/Address' },
      { icon: 'icon_group', name: '咨询反馈', path: 'pages/Feedback' },
      { icon: 'icon_notebook', name: '笔记本', path: 'pages/Notebook' },
      { icon: 'icon_settings', name: '设置', path: 'pages/Settings' }
    ];
  }

  build() {
    Column() {
      Row() {
        Image($r("app.media.head"))
          .size({
            width: this.AVATAR_SIZE,
            height: this.AVATAR_SIZE
          })
          .borderRadius(this.AVATAR_SIZE / 2)
          .margin({ right: 16 })

        Column() {
          Text('张三')
            .fontSize(24)
            .fontWeight(FontWeight.Bold)
            .margin({ bottom: 8 })
          Text('VIP会员')
            .fontSize(14)
            .fontColor(0x666666)
        }
        .alignItems(HorizontalAlign.Start)
        .layoutWeight(1)

        Button('编辑资料')
          .padding({
            top: 8,
            bottom: 8,
            left: 12,
            right: 12
          })
          .fontColor(0x333333)
          .border({
            width: 1,
            color: 0xE5E5E5
          })
          .backgroundColor(Color.White)
          .onClick(() => {
            router.pushUrl({ url: 'pages/UserPage' })
          })
      }
      .padding(16)
      .backgroundColor(Color.White)
      .margin({ bottom: 12 })

      List({ space: 0 }) {
        ForEach(this.functionItems, (item: FunctionItem) => {
          ListItem() {
            Row() {
              Image($r(`app.media.${item.icon}`))
                .size({ width: 24, height: 24 })
                .margin({ right: 16 })

              Text(item.name)
                .fontSize(18)
                .fontColor(0x333333)
                .layoutWeight(1)

              Text('>')
                .fontSize(16)
                .fontColor(0x999999)
            }
            .padding(16)
            .height(this.ITEM_HEIGHT)
            .width('100%')
            .onClick(() => {
              if (item.path) {
                router.pushUrl({ url: item.path });
              }
            })
          }
          .border({ width: 1, color: 0xF5F5F5 })
        })
      }
      .backgroundColor(Color.White)
    }
    .width('100%')
    .height('100%')
    .backgroundColor(0xF7F8FA)
  }
}