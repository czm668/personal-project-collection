import { promptAction, router } from '@kit.ArkUI'

@Entry
@Component
export struct LoginPage {
  @State username: string = '';
  @State password: string = '';
  @State inputFocus: boolean = false;

  build() {
    Column({ space: 20 }) {
      // 顶部图标
      Image($r('app.media.logo'))
        .width(120)
        .height(120)
        .margin({ top: '10%' })
        .borderRadius(60)
        .shadow({ radius: 12, color: '#007AFF', offsetX: 0, offsetY: 4 });

      Column({ space: 30 }) {
        TextInput({ placeholder: '请输入用户名', text: $$this.username })
          .width('100%')
          .height(50)
          .fontSize(18)
          .padding(10)
          .backgroundColor('#FFFFFF')
          .borderRadius(25)
          .placeholderColor('#999999')
          .caretColor('#007AFF')
          .onChange((value) => {
            this.username = value;
          })
          .onFocus(() => {
            this.inputFocus = true;
          })
          .onBlur(() => {
            this.inputFocus = false;
          });

        // 密码输入框
        TextInput({ placeholder: '请输入密码', text: $$this.password })
          .width('100%')
          .height(50)
          .type(InputType.Password)
          .fontSize(18)
          .padding(10)
          .backgroundColor('#FFFFFF')
          .borderRadius(25)
          .placeholderColor('#999999')
          .caretColor('#007AFF')
          .onChange((value) => {
            this.password = value;
          })
          .margin({ top: 10 });

        Row({ space: 20 }) {
          Button('登录')
            .width('60%')
            .height(50)
            .fontSize(20)
            .fontColor('#FFFFFF')
            .backgroundColor('#007AFF')
            .borderRadius(25)
            .opacity(this.username && this.password? 1 : 0.6)
            .onClick(() => {
              if (this.username === 'chenzhiming' && this.password === '123456') {
                router.pushUrl({ url: 'pages/HomePage' });
              } else {
                promptAction.showToast({
                  message: '用户名或密码错误',
                  duration: 2000,
                  bottom: '50%'
                });
              }
            });

          Button('重置')
            .width('30%')
            .height(50)
            .fontSize(20)
            .fontColor('#666666')
            .backgroundColor('#EEEEEE')
            .borderRadius(25)
            .onClick(() => {
              this.username = '';
              this.password = '';
            });
        }
        .width('100%')
        .margin({ top: 30 });
      }
      .width('100%')
      .padding(30)
      .backgroundColor('#F8F8F8')
      .borderRadius(20)
      .margin({ top: '15%' });

      Text('忘记密码？')
        .fontSize(16)
        .fontColor('#007AFF')
        .margin({ top: 30 })
        .onClick(() => {
        });
    }
    .width('100%')
    .height('100%')
    .padding(20)
    .backgroundColor(Color.White)
  }
}