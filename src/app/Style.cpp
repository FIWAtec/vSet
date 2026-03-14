#include "app/Style.h"

QString Style::mainStyleSheet()
{
    return R"(
        QWidget {
            background: #0f1115;
            color: #e8edf5;
            font-family: "Segoe UI", "SF Pro Display", Arial;
            font-size: 13px;
        }

        QMainWindow, QDialog {
            background: #0f1115;
        }

        QLabel#TitleLabel {
            font-size: 24px;
            font-weight: 700;
            color: #f5f7fb;
        }

        QLabel#SectionTitle {
            font-size: 16px;
            font-weight: 600;
            color: #f5f7fb;
        }

        QPushButton {
            background: #1f2530;
            border: 1px solid #2b3442;
            border-radius: 12px;
            padding: 10px 14px;
            min-height: 18px;
        }

        QPushButton:hover {
            background: #273142;
        }

        QPushButton:pressed {
            background: #18202b;
        }

        QPushButton#PrimaryButton {
            background: #2b6fff;
            border: 1px solid #4a86ff;
            color: white;
            font-weight: 600;
        }

        QPushButton#DangerButton {
            background: #422126;
            border: 1px solid #7a3942;
        }

        QLineEdit, QTextEdit, QPlainTextEdit, QListWidget, QSlider, QSpinBox {
            background: #131821;
            border: 1px solid #2b3442;
            border-radius: 12px;
            padding: 8px;
        }

        QGroupBox {
            border: 1px solid #232c38;
            border-radius: 18px;
            margin-top: 12px;
            padding-top: 16px;
            background: #131821;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 14px;
            padding: 0 6px;
            color: #c8d4e4;
        }

        QMenuBar {
            background: #0f1115;
            border-bottom: 1px solid #1f2530;
        }

        QMenuBar::item:selected, QMenu::item:selected {
            background: #273142;
        }

        QStatusBar {
            background: #0f1115;
            border-top: 1px solid #1f2530;
        }
    )";
}
