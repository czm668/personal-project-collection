import router from '@ohos.router';

interface UserInfoType {
  name: string;
  avatar: Resource;
  vipLevel: string;
  phone: string;
  email: string;
  birthday: string;
  gender: string;
}

@Entry
@Component
export struct UserInfo {
  @State userInfo: UserInfoType = {
    name: '张三',
    avatar: $r('app.media.head'),
    vipLevel: 'VIP会员',
    phone: '138****1234',
    email: 'example@mail.com',
    birthday: '1990-01-01',
    gender: '男'
  };

  @Builder
  createInfoItem(label: string, value: string) {
    Row() {
      Text(label)
        .width('30%')
        .textAlign(TextAlign.Start);
      Text(value)
        .width('70%')
        .textAlign(TextAlign.End);
    }
    .padding({ top: 5, bottom: 5 });
  }
  build() {
    Column({ space: 10 }) {
      Stack() {
        Image($r('app.media.bg'))
          .objectFit(ImageFit.Cover)
          .width('100%')
          .height(180)

        Row() {
          Image(this.userInfo.avatar)
            .size({ width: 80, height: 80 })
            .borderRadius(40)
            .margin({ right: 15 })

          Column() {
            Text(this.userInfo.name)
              .fontSize(20)
              .fontColor(Color.White)
            Text(this.userInfo.vipLevel)
              .fontSize(14)
              .fontColor(Color.White)
              .backgroundColor(0xFF9500)
              .padding(4)
              .borderRadius(8)
          }
        }
        .width('100%')
        .padding(16)
        .align(Alignment.BottomStart)
      }
      .height(180)

      Column() {
        this.createInfoItem('昵称', this.userInfo.name)
        this.createInfoItem('手机号', this.userInfo.phone)
        this.createInfoItem('邮箱', this.userInfo.email)
        this.createInfoItem('生日', this.userInfo.birthday)
        this.createInfoItem('性别', this.userInfo.gender)
      }
      .width('100%')
      .backgroundColor(Color.White)
      .padding(10)

      Button('保存修改')
        .width('90%')
        .height(45)
        .backgroundColor(0x007DFF)
        .fontColor(Color.White)
        .onClick(() => {
          console.info('保存用户信息');
          router.back();
        })
    }
    .width('100%')
    .padding(10)
    .backgroundColor(0xF5F5F5)
  }
}