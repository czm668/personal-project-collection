import router from '@ohos.router';
import promptAction from '@ohos.promptAction';

interface ExamType {
  id: number;
  name: string;
  description: string;
}

@Entry
@Component
export struct ExamBookingView {
  @State private selectedExamType: number = -1;
  @State private selectedDate: string = '';
  @State private selectedTimeSlot: string = '';
  @State private currentStep: number = 1;

  private examTypes: ExamType[] = [
    { id: 1, name: '科目一理论考试', description: '道路交通安全法律、法规和相关知识' },
    { id: 2, name: '科目二场地驾驶', description: '场地五项（倒车入库、侧方停车、曲线行驶、直角转弯、坡道定点停车与起步）' },
    { id: 3, name: '科目三路考', description: '实际道路驾驶技能' }
  ];

  private availableDates: string[] = [
    '2025年5月15日', '2025年5月16日', '2025年5月17日',
    '2025年5月18日', '2025年5月20日', '2025年5月22日'
  ];

  private timeSlots: string[] = [
    '上午 8:30 - 10:00', '上午 10:15 - 11:45',
    '下午 13:30 - 15:00', '下午 15:15 - 16:45'
  ];

  build() {
    Column({ space: 15 }) {
      // 顶部导航
      Row() {
        Button('返回')
          .onClick(() => {
            console.info(`Back button clicked, current step: ${this.currentStep}`);
            if (this.currentStep > 1) {
              this.currentStep--;
            } else {
              router.back();
            }
          })
          .width(80)
          .height(35)
          .backgroundColor('#FFFFFF')
          .borderRadius(8)
        Text('考试预约')
          .fontSize(20)
          .fontWeight(FontWeight.Bold)
          .margin({ left: 10 })
      }
      .width('100%')
      .height(60)
      .backgroundColor('#FFFFFF')
      .padding({ left: 15, right: 15 })

      // 步骤指示
      Row() {
        ForEach([1, 2, 3, 4] as number[], (step: number) => {
          Column() {
            Text(step.toString())
              .fontSize(14)
              .backgroundColor(step <= this.currentStep ? '#1677FF' : '#E5E5E5')
              .padding(8)
              .borderRadius(12)
            Text(step === 1 ? '选科目' : step === 2 ? '选日期' : step === 3 ? '选时间' : '确认')
              .fontSize(12)
              .margin({ top: 5 })
              .fontColor(step <= this.currentStep ? '#1677FF' : '#666666')
          }
          .width('25%')
          .alignItems(HorizontalAlign.Center)
        })
      }
      .width('100%')
      .backgroundColor('#FFFFFF')
      .padding({ left: 15, right: 15 })

      // 内容区域
      Column() {
        if (this.currentStep === 1) {
          this.renderExamTypeSelection();
        } else if (this.currentStep === 2) {
          this.renderDateSelection();
        } else if (this.currentStep === 3) {
          this.renderTimeSelection();
        } else if (this.currentStep === 4) {
          this.renderConfirmation();
        }
      }
      .width('100%')
      .margin({ left: 15, right: 15 })
      .height('70%')

      // 操作按钮
      if (this.currentStep < 4) {
        Button(this.currentStep === 1 ? '下一步' : '确认')
          .width('90%')
          .height(45)
          .backgroundColor('#1677FF')
          .fontColor('#FFFFFF')
          .onClick(() => {
            console.info(`Next button clicked at step ${this.currentStep}`);
            if (this.validateStep()) {
              console.info(`Validation passed, moving to step ${this.currentStep + 1}`);
              this.currentStep++;
            } else {
              console.info(`Validation failed at step ${this.currentStep}`);
              promptAction.showToast({ message: '请完成当前步骤选择' });
            }
          })
      } else {
        Button('提交预约')
          .width('90%')
          .height(45)
          .backgroundColor('#1677FF')
          .fontColor('#FFFFFF')
          .onClick(() => {
            console.info('Submit button clicked, sending booking request');
            console.info(`Booking details: ExamType=${this.selectedExamType}, Date=${this.selectedDate}, Time=${this.selectedTimeSlot}`);

            // 模拟提交预约
            promptAction.showToast({ message: '预约申请已提交' });
            router.back();
          })
      }
    }
    .width('100%')
    .height('100%')
    .backgroundColor('#F5F5F5')
    .padding({ top: 15 })
  }

  @Builder
  renderExamTypeSelection() {
    Column() {
      Text('请选择考试科目')
        .fontSize(16)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 10 })
      ForEach(this.examTypes, (type: ExamType) => {
        Button() {
          Row() {
            Text(type.name)
              .fontSize(16)
              .padding(15)
            if (this.selectedExamType === type.id) {
              Text('✔')
                .fontSize(18)
                .fontColor('#1677FF')
                .margin({ right: 15 })
            }
          }
          .width('100%')
          .justifyContent(FlexAlign.SpaceBetween)
        }
        .width('100%')
        .height(60)
        .onClick(() => {
          console.info(`Selected exam type: ${type.id}, name: ${type.name}`);
          this.selectedExamType = type.id;
        })
        .backgroundColor(this.selectedExamType === type.id ? '#E6F4FF' : '#FFFFFF')
        .borderRadius(8)
        .border({ width: 1, color: '#E0E0E0' })
        .margin({ bottom: 10 })
      }, (type: ExamType) => type.id.toString())
    }
    .onAppear(() => {
      console.info(`Exam type selection appeared, current selection: ${this.selectedExamType}`);
    })
  }

  @Builder
  renderDateSelection() {
    Column() {
      Text('请选择考试日期')
        .fontSize(16)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 10 })
      ForEach(this.availableDates, (date: string) => {
        Button() {
          Row() {
            Text(date)
              .fontSize(16)
              .padding(15)
            if (this.selectedDate === date) {
              Text('✔')
                .fontSize(18)
                .fontColor('#1677FF')
                .margin({ right: 15 })
            }
          }
          .width('100%')
          .justifyContent(FlexAlign.SpaceBetween)
        }
        .width('100%')
        .height(60)
        .onClick(() => {
          console.info(`Selected date: ${date}`);
          this.selectedDate = date;
        })
        .backgroundColor(this.selectedDate === date ? '#E6F4FF' : '#FFFFFF')
        .borderRadius(8)
        .border({ width: 1, color: '#E0E0E0' })
        .margin({ bottom: 10 })
      }, (date: string) => date)
    }
    .onAppear(() => {
      console.info(`Date selection appeared, current selection: ${this.selectedDate}`);
    })
  }

  @Builder
  renderTimeSelection() {
    Column() {
      Text('请选择考试时间')
        .fontSize(16)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 10 })
      ForEach(this.timeSlots, (slot: string) => {
        Button() {
          Row() {
            Text(slot)
              .fontSize(16)
              .padding(15)
            if (this.selectedTimeSlot === slot) {
              Text('✔')
                .fontSize(18)
                .fontColor('#1677FF')
                .margin({ right: 15 })
            }
          }
          .width('100%')
          .justifyContent(FlexAlign.SpaceBetween)
        }
        .width('100%')
        .height(60)
        .onClick(() => {
          console.info(`Selected time slot: ${slot}`);
          this.selectedTimeSlot = slot;
        })
        .backgroundColor(this.selectedTimeSlot === slot ? '#E6F4FF' : '#FFFFFF')
        .borderRadius(8)
        .border({ width: 1, color: '#E0E0E0' })
        .margin({ bottom: 10 })
      }, (slot: string) => slot)
    }
    .onAppear(() => {
      console.info(`Time selection appeared, current selection: ${this.selectedTimeSlot}`);
    })
  }

  @Builder
  renderConfirmation() {
    Column({ space: 15 }) {
      Text('预约信息确认')
        .fontSize(18)
        .fontWeight(FontWeight.Bold)
        .margin({ bottom: 10 })
      Row() {
        Text('考试科目：')
          .fontSize(16)
        Text(`${this.examTypes.find(t => t.id === this.selectedExamType)?.name || ''}`)
          .fontSize(16)
          .fontWeight(FontWeight.Medium)
      }
      .width('100%')
      .height(40)
      .backgroundColor('#FFFFFF')
      .padding(15)
      .borderRadius(8)
      Row() {
        Text('考试日期：')
          .fontSize(16)
        Text(`${this.selectedDate}`)
          .fontSize(16)
          .fontWeight(FontWeight.Medium)
      }
      .width('100%')
      .height(40)
      .backgroundColor('#FFFFFF')
      .padding(15)
      .borderRadius(8)
      Row() {
        Text('考试时间：')
          .fontSize(16)
        Text(`${this.selectedTimeSlot}`)
          .fontSize(16)
          .fontWeight(FontWeight.Medium)
      }
      .width('100%')
      .height(40)
      .backgroundColor('#FFFFFF')
      .padding(15)
      .borderRadius(8)
      Text('请携带有效证件按时参加考试')
        .fontSize(14)
        .fontColor('#666666')
        .margin({ top: 10 })
        .padding(15)
        .backgroundColor('#FFF7E6')
        .borderRadius(8)
    }
    .backgroundColor('#FFFFFF')
    .padding(15)
    .borderRadius(8)
    .onAppear(() => {
      console.info(`Confirmation page appeared with selection:
        ExamType=${this.selectedExamType},
        Date=${this.selectedDate},
        Time=${this.selectedTimeSlot}`);
    })
  }

  validateStep() {
    let isValid = false;

    switch (this.currentStep) {
      case 1:
        isValid = this.selectedExamType !== -1;
        console.info(`Step 1 validation: selectedExamType=${this.selectedExamType}, valid=${isValid}`);
        break;
      case 2:
        isValid = this.selectedDate.trim() !== '';
        console.info(`Step 2 validation: selectedDate=${this.selectedDate}, valid=${isValid}`);
        break;
      case 3:
        isValid = this.selectedTimeSlot.trim() !== '';
        console.info(`Step 3 validation: selectedTimeSlot=${this.selectedTimeSlot}, valid=${isValid}`);
        break;
      default:
        isValid = true;
        console.info(`Step ${this.currentStep} validation: default true`);
    }

    return isValid;
  }
}