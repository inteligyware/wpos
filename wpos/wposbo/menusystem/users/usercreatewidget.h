// file      :  usercreatewidget.h
// birth     :  6/2/2003
// copyright :  Copyright (c) 2003 by Napsis S.L.
// copyright :  Copyright (c) 2016-2024 WireSens Inc.
// author    :  Carlos Manzanedo Rueda, Gilles Bene Pougoue
// contact   :  contact@wiresens.com - +237 697 02 63 76

#ifndef USER_CREATION_WIDGET_H
#define USER_CREATION_WIDGET_H

#include "usereditionwidget.h"

class QString;
class FX2000EnrollWidget;

class UserCreationWidget : public UserEditionWidget {
    Q_OBJECT
public:
    enum State {
        Creation = 1,
        Enrollment = 2,
        AccesControl = 3
    };

    explicit UserCreationWidget(UserModule* _user_mod, QWidget* parent = 0, const QString& name = QString {});

protected slots:
    virtual void apply() override;
    virtual void cancel() override;

    void flushExpectingData();
    void fingerprintAcquireSlot();
    void fingerprintReadSlot(const QString& xml_str);

protected:
    virtual void doInitialize() override;
    virtual void doActualize() override;
    virtual bool enroll();
    virtual bool setReturnApplication();

private:
    State state { Creation }; // 1 for data, 2 to acquire fingerprint, 3 to ACL
};

#endif // USER_CREATION_WIDGET_H
