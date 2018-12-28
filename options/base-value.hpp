#pragma once

#include "bundle.hpp"
#include "slider.hpp"
#include "connector.hpp"
#include "metatype.hpp"

#include "export.hpp"
#include "compat/macros.hpp"
#include "value-traits.hpp"

#include <QObject>
#include <QString>
#include <QList>
#include <QPointF>
#include <QVariant>

#include <utility>

#define OTR_OPTIONS_SLOT(t) void setValue(t datum) { store_(datum); }
#define OTR_OPTIONS_SIGNAL(t) void valueChanged(t) const

namespace options {

class OTR_OPTIONS_EXPORT value_ : public QObject
{
    Q_OBJECT

    template<typename t>
    using signal_sig = void(value_::*)(cv_qualified<t>) const;

public:
    QString name() const { return self_name; }
    value_(bundle const& b, const QString& name);
    ~value_() override;

    // no C++17 "constexpr inline" for data declarations in MSVC
    template<typename t>
    static constexpr auto value_changed()
    {
        return static_cast<signal_sig<t>>(&value_::valueChanged);
    }

signals:
    OTR_OPTIONS_SIGNAL(double);
    OTR_OPTIONS_SIGNAL(float);
    OTR_OPTIONS_SIGNAL(int);
    OTR_OPTIONS_SIGNAL(bool);
    OTR_OPTIONS_SIGNAL(const QString&);
    OTR_OPTIONS_SIGNAL(const slider_value&);
    OTR_OPTIONS_SIGNAL(const QPointF&);
    OTR_OPTIONS_SIGNAL(const QVariant&);

    OTR_OPTIONS_SIGNAL(const QList<double>&);
    OTR_OPTIONS_SIGNAL(const QList<float>&);
    OTR_OPTIONS_SIGNAL(const QList<int>&);
    OTR_OPTIONS_SIGNAL(const QList<bool>&);
    OTR_OPTIONS_SIGNAL(const QList<QString>&);
    OTR_OPTIONS_SIGNAL(const QList<slider_value>&);
    OTR_OPTIONS_SIGNAL(const QList<QPointF>&);

protected:
    bundle b;
    QString self_name;

    virtual void store_variant(const QVariant& x) = 0;

    template<typename t>
    void store_(const t& datum)
    {
        using traits = detail::value_traits<t>;
        store_variant(traits::qvariant_from_value(datum));
    }

public slots:
    OTR_OPTIONS_SLOT(double)
    OTR_OPTIONS_SLOT(int)
    OTR_OPTIONS_SLOT(bool)
    OTR_OPTIONS_SLOT(const QString&)
    OTR_OPTIONS_SLOT(const slider_value&)
    OTR_OPTIONS_SLOT(const QPointF&)
    OTR_OPTIONS_SLOT(const QVariant&)

    OTR_OPTIONS_SLOT(const QList<double>&)
    OTR_OPTIONS_SLOT(const QList<float>&)
    OTR_OPTIONS_SLOT(const QList<int>&)
    OTR_OPTIONS_SLOT(const QList<bool>&)
    OTR_OPTIONS_SLOT(const QList<QString>&)
    OTR_OPTIONS_SLOT(const QList<slider_value>&)
    OTR_OPTIONS_SLOT(const QList<QPointF>&)

    virtual void set_to_default() = 0;
    virtual void notify() const = 0;

    friend void ::options::detail::set_value_to_default(value_* val);
};

} //ns options
