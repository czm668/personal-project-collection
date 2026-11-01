import { router } from "@kit.ArkUI";

@Entry
@Component
export struct AccountSecurity {
  build() {
    Column() {
      AccountSecurity.BuildHeader('账号与安全')
      this.BuildNavItem('手机号', '138****1234', 'phonePage')
      this.BuildNavItem('邮箱', 'example@mail.com', 'emailPage')
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5')
  }

  @Builder
  static BuildHeader(title: string) {
    Row() {
      Image($r('app.media.back'))
        .width(24)
        .height(24)
        .onClick(() => router.back())

      Text(title)
        .fontSize(20)
        .margin({ left: 16 })

      Blank()
    }
    .padding(16)
    .backgroundColor(Color.White)
  }

  @Builder
  BuildNavItem(label: string, value: string, pageName: string) {
    Row() {
      Text(label).fontSize(16)
      Blank()
      Text(value).fontSize(14).fontColor('#999999')
      Image($r('app.media.arrow_right'))
        .width(16)
        .height(16)
        .margin({ left: 8 })
    }
    .padding(16)
    .backgroundColor(Color.White)
    .width('100%')
    .onClick(() => {
      router.pushUrl({ url: pageName })
    })
  }
}

@Component
struct PhonePage {
  build() {
    Column() {
      AccountSecurity.BuildHeader('手机号设置')
      Column() {
        Text('当前绑定手机号：138****1234')
          .fontSize(16)
        Button('更换手机号')
          .margin({ top: 24 })
          .width('50%')
      }
      .padding(16)
    }
    .backgroundColor('#F5F5F5')
  }
}

@Component
struct EmailPage {
  build() {
    Column() {
      AccountSecurity.BuildHeader('邮箱设置')

      Column() {
        Text('当前绑定邮箱：example@mail.com')
          .fontSize(16)
        Button('更换邮箱')
          .margin({ top: 24 })
          .width('50%')
      }
      .padding(16)
    }
    .backgroundColor('#F5F5F5')
  }
}