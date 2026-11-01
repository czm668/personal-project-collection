import { router } from '@kit.ArkUI'

@Entry
@Component
struct StartPage {
  private timerId: number = -1

  aboutToAppear(): void {
    this.timerId = setTimeout(() => {
      router.replaceUrl({
        url: 'pages/LoginPage'
      })
    }, 3000)
  }

  aboutToDisappear(): void {
    clearTimeout(this.timerId)
  }

  build() {
    Column(){
      Image($r('app.media.Start'))
        .width('100%')
        .height('100%')
    }
    .width('100%')
    .height('100%')
  }
}