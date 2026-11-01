import router from '@ohos.router';

interface GuideItem {
  id: number;
  icon: Resource;
  title: string;
  description: string;
}

@Entry
@Component
export struct BusinessGuideView {
  private guideItems: GuideItem[] = [
    {
      id: 1,
      icon: $r('app.media.ic_account'),
      title: '账户管理',
      description: '查看账户信息、修改个人资料、绑定银行卡等操作指南'
    },
    {
      id: 2,
      icon: $r('app.media.ic_payment'),
      title: '支付方式',
      description: '添加和管理支付方式、设置默认支付渠道、查看支付记录'
    },
    {
      id: 3,
      icon: $r('app.media.ic_transfer'),
      title: '转账汇款',
      description: '同行/跨行转账、批量转账、转账限额设置及到账时间说明'
    },
    {
      id: 4,
      icon: $r('app.media.ic_investment'),
      title: '投资理财',
      description: '基金、理财产品购买流程、风险评估及收益查询指南'
    },
    {
      id: 5,
      icon: $r('app.media.ic_loan'),
      title: '贷款服务',
      description: '个人贷款、企业贷款申请条件及流程、还款方式介绍'
    },
    {
      id: 6,
      icon: $r('app.media.ic_card'),
      title: '信用卡服务',
      description: '信用卡申请进度查询、账单查询、还款及积分兑换指南'
    },
    {
      id: 7,
      icon: $r('app.media.ic_service'),
      title: '客户服务',
      description: '联系客服、常见问题解答、意见反馈渠道介绍'
    },
    {
      id: 8,
      icon: $r('app.media.ic_setting'),
      title: '安全设置',
      description: '登录密码修改、手势密码设置、账户安全锁使用指南'
    }
  ];

  build() {
    Column() {
      Row({ space: 5 }) {
        Image($r('app.media.back_icon'))
          .width(24)
          .height(24)
          .margin({ left: 15 })
          .onClick(() => router.back())

        Text('业务指南')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')

      Column() {
        List() {
          ForEach(this.guideItems, (item: GuideItem) => {
            ListItem() {
              Column() {
                Row() {
                  Image(item.icon)
                    .width(40)
                    .height(40)
                    .margin({ right: 15 })

                  Column({ space: 5 }) {
                    Text(item.title)
                      .fontSize(16)
                      .fontWeight(FontWeight.Medium)
                    Text(item.description)
                      .fontSize(12)
                      .fontColor('#666666')
                      .margin({ top: 5 })
                  }
                  .width('80%')

                  Image($r('app.media.ic_arrow_right'))
                    .width(16)
                    .height(16)
                }
                .width('100%')
                .height(80)
                .padding({ left: 15, right: 15 })
                .onClick(() => {
                  router.pushUrl({
                    url: 'pages/BusinessGuideDetail',
                    params: { guideId: item.id }
                  });
                })
              }
            }
          })
        }
        .width('100%')
        .height('80%')
        .margin({ top: 5 })
      }
      .width('100%')
      .padding(20)
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5')
  }
}

@Preview
struct BusinessGuideViewPreview {
  build() {
    BusinessGuideView()
  }
}