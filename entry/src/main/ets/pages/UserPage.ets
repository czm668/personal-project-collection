import { router } from '@kit.ArkUI'

@Entry
@Component
export struct UserPage {
  @State avatarUrl: string = ""
  @State name: string = "张三"
  @State gender: string = "男"
  @State description: string = ""
  @State phoneNumber: string = "XXXXXXXXXXX"

  build() {
    Column() {
      Row() {
        Button('返回>')
          .margin({ right: 10 })
          .backgroundColor(0xFFFFFF)
          .borderRadius(5)
          .border({ width: 1, color: 0xCCCCCC })
          .fontColor(Color.Black)
          .onClick(()=>{
            router.pushUrl({
              url:'pages/HomePage'
            })
          })
      }.justifyContent(FlexAlign.End).padding({ top: 10 })
      .width('100%')


      Row() {
        Text("账号管理")
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
      }
      .width('100%')
      .height(50)
      .padding({ left: 16 })

      Row() {
        Text("头像")
          .fontSize(16)
        if (this.avatarUrl) {
          Image(this.avatarUrl)
            .width(60)
            .height(60)
            .clipShape(Circle())
        } else {
          Image($r("app.media.head"))
            .width(60)
            .height(60)
            .clipShape(Circle())
        }
      }
      .width('100%')
      .height(80)
      .padding({ left: 16, right: 16 })

      Row() {
        Text("姓名")
          .fontSize(16)
        Text(this.name)
          .fontSize(16)
          .fontColor(Color.Black)
      }
      .width('100%')
      .height(60)
      .padding({ left: 16, right: 16 })

      Row() {
        Text("性别")
          .fontSize(16)
        Text(this.gender)
          .fontSize(16)
      }
      .width('100%')
      .height(60)
      .padding({ left: 16, right: 16 })

      Row() {
        Text("描述")
          .fontSize(16)
        Text(this.description)
          .fontSize(16)
      }
      .width('100%')
      .height(60)
      .padding({ left: 16, right: 16 })

      Row() {
        Text("手机号")
          .fontSize(16)
        Text(this.phoneNumber)
          .fontSize(16)
      }
      .width('100%')
      .height(60)
      .padding({ left: 16, right: 16 })

      Button("退出登录")
        .width('90%')
        .height(50)
        .backgroundColor(Color.Red)
        .fontSize(16)
        .fontColor(Color.White)
        .borderRadius(10)
        .margin({ top: 20 })
        .onClick(() => {
          router.pushUrl({
            url:'pages/LoginPage'
          })
        })
    }
    .width('100%')
    .height('100%')
    .backgroundColor(Color.White)
  }
}