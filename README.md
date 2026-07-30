<div align="center">

# 🅿️ Smart Parking System

### 15-Slot Car Park with IR Occupancy, Per-Slot Lights, Auto Gates & Live LCD Count

![Arduino](https://img.shields.io/badge/Arduino-Mega-00979D?logo=arduino&logoColor=white)
![Domain](https://img.shields.io/badge/Domain-Smart%20City-1f6feb)
![Type](https://img.shields.io/badge/Type-Embedded-8957e5)
[![License: MIT](https://img.shields.io/badge/License-MIT-00C896.svg)](LICENSE)
[![GitHub](https://img.shields.io/badge/GitHub-eahmeddarwish-181717?logo=github)](https://github.com/eahmeddarwish/smart-parking-system)
![Status](https://img.shields.io/badge/status-refactored%20%26%20documented-brightgreen)

**Built by [Ahmed Darwish](mailto:eahmeddarwish@gmail.com)**

[📖 How it works](#-how-it-works--كيف-يعمل) · [⚠️ Honest Limitations](#-honest-limitations--محدوديات-صادقة) · [🚀 Quick Start](#-quick-start--البدء-السريع) · [⭐ Star on GitHub](https://github.com/eahmeddarwish/smart-parking-system)

</div>

---

## 🌍 Overview | نظرة عامة

**[English]**
A 15-slot smart car park (3 sections × 5 slots) on an Arduino Mega. Every slot
has an **IR sensor** and a **Red/Green LED**: green when free, red when taken.
**Entrance and exit IR sensors** raise two **servo gates** for arriving/leaving
cars, and a **20×4 I²C LCD** shows how many slots are free in each section — a
compact model of a real guided-parking system.

**[العربية]**
موقفٌ ذكيٌّ بـ 15 مكانًا (3 أقسام × 5 أماكن) على Arduino Mega. كل مكانٍ له **حسّاس
IR** و**LED أحمر/أخضر**: أخضر إذا كان فارغًا وأحمر إذا مشغولًا. **حسّاسا IR للمدخل
والمخرج** يرفعان **بوابتَي سيرفو** للسيارات الداخلة/الخارجة، وشاشة **I²C مقاس 20×4**
تعرض عدد الأماكن الفارغة في كل قسم — نموذجٌ مصغّرٌ لنظام إرشاد مواقف حقيقي.

---

## 🔧 What was cleaned up | ما الذي نُظِّف

**[English]**
The original repeated the same `if (digitalRead(x)) {…}` block **15 times** with
a tangled sensor→LED mapping (~200 lines). This version puts sensors and LEDs in
**arrays** and a **slot map**, so all 15 slots run in one small loop, the gates
are one reusable function, and the free-slot count per section is computed
directly. Same wiring, a fraction of the code.

**[العربية]**
النسخة الأصلية كرّرت نفس بلوك `if (digitalRead(x)) {…}` **15 مرة** مع ربطٍ متشابكٍ
بين الحسّاس والـ LED (~200 سطر). هذه النسخة تضع الحسّاسات والـ LEDs في **مصفوفات**
و**خريطة أماكن**، فتعمل الأماكن الـ15 في حلقةٍ صغيرةٍ واحدة، والبوابات دالةٌ واحدةٌ
قابلةٌ لإعادة الاستخدام، وعدّ الأماكن الفارغة لكل قسمٍ يُحسب مباشرة.

---

## ✨ Key Features | أهم المزايا

| Feature | Description |
|---|---|
| 🅿️ **15 slots, 3 sections** | Per-slot IR occupancy detection |
| 🚦 **Red/Green per slot** | Instant visual free/occupied status |
| 🚧 **Auto entry/exit gates** | Servo gates on entrance & exit IR |
| 🖥️ **20×4 LCD count** | Free slots per section, live |
| 🧮 **Array-driven** | Arrays + slot map instead of 15 copy-pasted blocks |

---

## 🔬 How it works | كيف يعمل

```
each slot:  IR blocked? → RED on / GREEN off  (else GREEN on)
section free count = slots not blocked (per group of 5) → LCD
entrance/exit IR blocked → open that servo gate 4 s → close
```

---

## 🧰 Hardware Used | العتاد المستخدم

| Component | Role |
|---|---|
| Arduino **Mega 2560** | 15 slots need the extra I/O |
| 15 × IR sensors | Slot occupancy |
| 15 × Red + 15 × Green LEDs | Slot status |
| 2 × IR sensors + 2 × servos | Entrance / exit gates |
| 20×4 I²C LCD | Free-slot display |

---

## 🚀 Quick Start | البدء السريع

1. Wire slots to the pins in the config arrays (Mega), gates' servos to **3/4**,
   entrance/exit IR to **52/53**, LCD to **SDA/SCL (20/21)**.
2. Install **LiquidCrystal_I2C** (and the built-in **Servo**).
3. Upload `src/smart_parking_system/smart_parking_system.ino`.
4. If your IR modules read HIGH when blocked, set `IR_ACTIVE_LOW = false`.

---

## ⚙️ Configuration | الإعدادات

| Setting | Meaning |
|---|---|
| `slotSensor[] / slotGreen[] / slotRed[]` | Per-slot pin arrays |
| `ENTRANCE_IR / EXIT_IR` | Gate trigger sensors (52 / 53) |
| `ENTRANCE_SERVO / EXIT_SERVO` | Gate servos (3 / 4) |
| `GATE_OPEN / GATE_CLOSED / GATE_MS` | Gate angles + open time |
| `IR_ACTIVE_LOW` | Flip if your IR reads HIGH when blocked |

---

## 📁 Project Structure | هيكل المشروع

```
.
├── src/
│   └── smart_parking_system/
│       └── smart_parking_system.ino
├── .gitignore
└── LICENSE
```

---

## ⚠️ Honest Limitations | محدوديات صادقة

**[English]**
- **Blocking gates.** Raising a gate uses `delay(4 s)`, so slot LEDs freeze while
  a gate is open; a `millis()` gate scheduler is the clean fix (roadmap).
- **No entry/exit counting logic** beyond the gate — occupancy is read purely
  from slot sensors.
- **IR sensors are binary + alignment-sensitive**; sunlight can cause false
  reads. Ultrasonic slot sensors would be more robust.
- **Needs a Mega** for the pin count; an Uno can't host 15 slots directly.

**[العربية]**
- **بوابات حاجبة**: رفع البوابة يستخدم `delay(4 ثوانٍ)`، فتتجمّد لمبات الأماكن أثناء
  فتح البوابة؛ والحل النظيف جدولة بالـ `millis()` (خطة مستقبلية).
- **بلا منطق عدّ دخول/خروج** غير البوابة — الإشغال يُقرأ من حسّاسات الأماكن فقط.
- **حسّاسات IR ثنائيةٌ وحسّاسةٌ للمحاذاة**؛ وضوء الشمس قد يسبّب قراءاتٍ خاطئة.
  حسّاسات فوق صوتيةٌ للأماكن أكثر متانة.
- **يحتاج Mega** لعدد الأطراف؛ Uno لا يكفي لـ15 مكانًا مباشرة.

---

## 🗺️ Roadmap | خطط التطوير

- [x] **Phase 1** — 15 slots, LEDs, auto gates, per-section LCD count *(current)*
- [ ] **Phase 2** — Non-blocking `millis()` gate control
- [ ] **Phase 3** — Total-count + "FULL" sign + nearest-free-slot guidance
- [ ] **Phase 4** — Log occupancy over time / send to an IoT dashboard

---

## 👤 Author | المطور

<div align="center">

**Ahmed Darwish**

*Electrical & Computer Engineer | Python · Arduino · Raspberry Pi · AI/ML*

[![Email](https://img.shields.io/badge/Email-eahmeddarwish%40gmail.com-EA4335?logo=gmail&logoColor=white)](mailto:eahmeddarwish@gmail.com)
[![GitHub](https://img.shields.io/badge/GitHub-eahmeddarwish-181717?logo=github)](https://github.com/eahmeddarwish)

</div>

---

## 📄 License

Licensed under the **MIT License** — see [LICENSE](LICENSE).

---

<div align="center">

⭐ **If this project is useful, please give it a star on GitHub!** ⭐

*Made with ❤️ by Ahmed Darwish*

</div>
