/***************************************************************************
  qgssettingsentry.h
  --------------------------------------
  Date                 : February 2021
  Copyright            : (C) 2021 by Damiano Lombardi
  Email                : damiano at opengis dot ch
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef QGSSETTINGSENTRY_H
#define QGSSETTINGSENTRY_H

#include <QString>
#include <QColor>
#include <limits>

#include "qgis.h"
#include "qgis_core.h"
#include "qgis_sip.h"
#include "qgssettings.h"

class QgsSettingsTreeNode;


/**
 * \ingroup core
 * \class QgsSettingsEntryBase
 *
 * \brief Represent settings entry and provides methods for reading and writing settings values.
 * Different subclasses are provided for different settings types with metainformations
 * to validate set values and provide more accurate settings description for the gui.
 *
 * \since QGIS 3.20
 */
class CORE_EXPORT QgsSettingsEntryBase
{

#ifdef SIP_RUN
    SIP_CONVERT_TO_SUBCLASS_CODE
    if ( dynamic_cast< QgsSettingsEntryVariant * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryVariant;
    else if ( dynamic_cast< QgsSettingsEntryString * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryString;
    else if ( dynamic_cast< QgsSettingsEntryStringList * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryStringList;
    else if ( dynamic_cast< QgsSettingsEntryVariantMap * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryVariantMap;
    else if ( dynamic_cast< QgsSettingsEntryBool * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryBool;
    else if ( dynamic_cast< QgsSettingsEntryInteger * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryInteger;
    else if ( dynamic_cast< QgsSettingsEntryDouble * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryDouble;
    else if ( dynamic_cast< QgsSettingsEntryColor * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryColor;
    else if ( dynamic_cast< QgsSettingsEntryBase * >( sipCpp ) )
      sipType = sipType_QgsSettingsEntryBase;
    else
      sipType = NULL;
    SIP_END
#endif

  public:

    /**
     * Transforms a dynamic key part string to list
     * \since QGIS 3.26
     */
    static QStringList dynamicKeyPartToList( const QString &dynamicKeyPart );

    /**
     * Constructor for QgsSettingsEntryBase.
     *
     * \arg key specifies the key of the settings.
     * \arg section specifies the section.
     * \arg defaultValue specifies the default value for the settings entry.
     * \arg description specifies a description for the settings entry.
     * \arg options specifies the options for the settings entry.
     */
    QgsSettingsEntryBase( const QString &key,
                          const QString &section,
                          const QVariant &defaultValue = QVariant(),
                          const QString &description = QString(),
                          Qgis::SettingsOptions options = Qgis::SettingsOptions() )
      : mKey( QStringLiteral( "%1/%2" ).arg( section, key ) )
      , mDefaultValue( defaultValue )
      , mDescription( description )
      , mOptions( options )
    {}

    /**
     * Constructor for QgsSettingsEntryBase.
     *
     * \arg key specifies the key of the settings.
     * \arg parent specifies the parent in the tree of settings.
     * \arg defaultValue specifies the default value for the settings entry.
     * \arg description specifies a description for the settings entry.
     * \arg options specifies the options for the settings entry.
     * \throws QgsSettingsException if the number of given parent named items doesn't match the complete key definition
     */
    QgsSettingsEntryBase( const QString &key,
                          QgsSettingsTreeNode *parentTreeElement,
                          const QVariant &defaultValue = QVariant(),
                          const QString &description = QString(),
                          Qgis::SettingsOptions options = Qgis::SettingsOptions() ) SIP_THROW( QgsSettingsException );

    /**
     * Destructor for QgsSettingsEntryBase.
     */
    virtual ~QgsSettingsEntryBase();

    /**
     * Returns settings entry key.
     *
     * \arg dynamicKeyPart specifies the dynamic part of the settings key.
     */
    QString key( const QString &dynamicKeyPart = QString() ) const;

    /**
     * Returns settings entry key.
     *
     * \arg dynamicKeyParts specifies the list of dynamic parts of the settings key.
     */
    QString key( const QStringList &dynamicKeyPartList ) const;

    /**
     * Returns TRUE if the provided key match the settings entry.
     *
     * This is useful for settings with dynamic keys. For example this permits one to check that
     * the settings key "NewsFeed/httpsfeedqgisorg/27/content" is valid for the settings entry
     * defined with the key "NewsFeed/%1/%2/content"
     *
     * \arg key to check
     */
    bool keyIsValid( const QString &key ) const;

    /**
     * Returns settings entry defining key.
     * For dynamic settings it return the key with the placeholder for dynamic part
     * included. For non-dynamic settings returns the same as key().
     */
    QString definitionKey() const;

    /**
     * Returns TRUE if a part of the settings key is built dynamically.
     */
    bool hasDynamicKey() const;

    /**
     * Returns the settings options
     * \since QGIS 3.26
     */
    Qgis::SettingsOptions options() const {return mOptions;}

    /**
     * Returns TRUE if the settings is contained in the underlying QSettings.
     *
     * \arg dynamicKeyPart specifies the dynamic part of the settings key.
     */
    bool exists( const QString &dynamicKeyPart = QString() ) const;

    /**
     * Returns TRUE if the settings is contained in the underlying QSettings.
     *
     * \arg dynamicKeyParts specifies the list of dynamic parts of the settings key.
     */
    bool exists( const QStringList &dynamicKeyPartList ) const;

    /**
     * Returns the origin of the setting if it exists
     * \note it will return Qgis::SettingsOrigin::Any if the key doesn't exist
     * \since QGIS 3.30
     */
    Qgis::SettingsOrigin origin( const QStringList &dynamicKeyPartList ) const;

    /**
     * Removes the settings from the underlying QSettings.
     *
     * \arg dynamicKeyPart specifies the dynamic part of the settings key.
     */
    void remove( const QString &dynamicKeyPart = QString() ) const;

    /**
     * Removes the settings from the underlying QSettings.
     *
     * \arg dynamicKeyParts specifies the list of dynamic parts of the settings key.
     */
    void remove( const QStringList &dynamicKeyPartList ) const;

    /**
     * Returns settings section. The settings section of the parent group is returned if available.
     * \deprecated since QGIS 3.26 the key is entirely self-defined
     */
    Q_DECL_DEPRECATED QgsSettings::Section section() const {return QgsSettings::NoSection;}

    /**
     * Set settings value.
     *
     * The \a value to set.
     * \arg dynamicKeyPart specifies the dynamic part of the settings key.
     * \deprecated since QGIS 3.26 use setVariantValuePrivate or an implementation setValue instead
     */
    Q_DECL_DEPRECATED virtual bool setVariantValue( const QVariant &value, const QString &dynamicKeyPart = QString() ) const SIP_DEPRECATED;

    /**
     * Set settings value.
     *
     * The \a value to set.
     * \arg dynamicKeyParts specifies the list of dynamic parts of the settings key.
     * \deprecated since QGIS 3.26 use setVariantValuePrivate or an implementation setValue instead
     */
    Q_DECL_DEPRECATED virtual bool setVariantValue( const QVariant &value, const QStringList &dynamicKeyPartList ) const SIP_DEPRECATED;

    //! Returns settings value with \arg dynamicKeyPart specifying the dynamic part of the settings key.
    QVariant valueAsVariant( const QString &dynamicKeyPart = QString() ) const;

    //! Returns settings value with \arg dynamicKeyPart specifying the dynamic part of the settings key.
    QVariant valueAsVariant( const QStringList &dynamicKeyPartList ) const;

    /**
     * Returns settings value with a \a defaultValueOverride
     * \since QGIS 3.26
     */
    QVariant valueAsVariantWithDefaultOverride( const QVariant &defaultValueOverride, const QString &dynamicKeyPart = QString() ) const;

    /**
     * Returns settings value.
     *
     * \arg dynamicKeyPartList specifies the list of dynamic parts of the settings key.
     * \arg defaultValueOverride if valid is used instead of the normal default value.
     * \since QGIS 3.26
     */
    QVariant valueAsVariantWithDefaultOverride( const QVariant &defaultValueOverride, const QStringList &dynamicKeyPartList ) const;

    /**
     * Returns settings value with an optional default value override
     * \deprecated since QGIS 3.26 use valueAsVariantWithDefaultOverride instead
     */
    Q_DECL_DEPRECATED QVariant valueAsVariant( const QString &dynamicKeyPart, bool useDefaultValueOverride, const QVariant &defaultValueOverride ) const SIP_DEPRECATED;

    /**
     * Returns settings value with an optional default value override
     * \deprecated since QGIS 3.26 use valueAsVariantWithDefaultOverride instead
     */
    Q_DECL_DEPRECATED QVariant valueAsVariant( const QStringList &dynamicKeyPartList, bool useDefaultValueOverride, const QVariant &defaultValueOverride ) const SIP_DEPRECATED;


    /**
     * Returns settings default value.
     */
    QVariant defaultValueAsVariant() const;

    /**
     * Returns the settings entry type.
     */
    virtual Qgis::SettingsType settingsType() const {return Qgis::SettingsType::Custom;}
    // This cannot be pure virtual otherwise SIP is failing

    /**
     * Returns the settings entry description.
     */
    QString description() const;

    /**
     * Returns the former value of the settings if it has been enabled in the options.
     * Returns the current value (or default) if there is no former value.
     * \since QGIS 3.26
     */
    QVariant formerValueAsVariant( const QString &dynamicKeyPart ) const;

    /**
     * Returns the former value of the settings if it has been enabled in the options
     * Returns the current value (or default) if there is no former value.
     * \since QGIS 3.26
     */
    QVariant formerValueAsVariant( const QStringList &dynamicKeyPartList ) const;

    /**
     * Copies the value from a given key if it exists.
     * \param key the key to copy the setting value from.
     * \param removeSettingAtKey if TRUE, the setting at the old key will be removed.
     * \returns TRUE if the key exists and the setting value could be copied.
     * \since QGIS 3.30
     */
    bool copyValueFromKey( const QString &key, bool removeSettingAtKey = false ) const {return copyValueFromKey( key, {}, removeSettingAtKey );}

    /**
     * Copies the value from a given key if it exists.
     * \param key the key to copy the setting value from.
     * \param dynamicKeyPartList is the optional dynamic key part to determine the key. It must be the same for origin and destination keys.
     * \param removeSettingAtKey if TRUE, the setting at the old key will be removed.
     * \returns TRUE if the key exists and the setting value could be copied.
     * \since QGIS 3.30
     */
    bool copyValueFromKey( const QString &key, const QStringList &dynamicKeyPartList, bool removeSettingAtKey = false ) const;

    /**
     * Copies the settings to the given key.
     * \param key the key to copy the setting value to.
     * \param dynamicKeyPartList is the optional dynamic key part to determine the key. It must be the same for origin and destination keys.
     * \since QGIS 3.30
     */
    void copyValueToKey( const QString &key, const QStringList &dynamicKeyPartList = QStringList() ) const;

    /**
    * Returns the parent tree element
    * \since QGIS 3.30
    */
    QgsSettingsTreeNode *parent() const {return mParentTreeElement;}

  protected:

    /**
     * Sets the settings value with a variant value.
     * This should be called from any implementation as it takes care of actually calling QSettings
     * \since QGIS 3.26
     */
    bool setVariantValuePrivate( const QVariant &value, const QStringList &dynamicKeyPartList = QStringList() ) const;

  private:
    QString formerValuekey( const QStringList &dynamicKeyPartList ) const;

    QString completeKeyPrivate( const QString &key, const QStringList &dynamicKeyPartList ) const;

    QgsSettingsTreeNode *mParentTreeElement = nullptr;
    QString mKey;
    QVariant mDefaultValue;
    QString mDescription;
    Qgis::SettingsOptions mOptions;
};


/**
 * \ingroup core
 * \class QgsSettingsEntryByReference
 *
 * \brief Base abstract class for settings entry which are passed by reference
 * \see QgsSettingsEntryBase
 * \see QgsSettingsEntryByValue
 *
 * \since QGIS 3.26
 */
template<class T>
class QgsSettingsEntryByReference : public QgsSettingsEntryBase
{
  public:

    /**
     * Constructor for QgsSettingsEntryByReference.
     *
     * \arg key specifies the key of the settings.
     * \arg parent specifies the parent in the tree of settings.
     * \arg defaultValue specifies the default value for the settings entry.
     * \arg description specifies a description for the settings entry.
     * \arg optionss specifies the options for the settings entry.
     * \throws QgsSettingsException if the number of given parent named items doesn't match the complete key definition
     */
    QgsSettingsEntryByReference( const QString &key,
                                 QgsSettingsTreeNode *parent,
                                 const T &defaultValue,
                                 const QString &description = QString(),
                                 Qgis::SettingsOptions options = Qgis::SettingsOptions() )
      : QgsSettingsEntryBase( key, parent, defaultValue, description, options )
    {}

    /**
     * Constructor for QgsSettingsEntryByReference.
     *
     * \arg key specifies the key of the settings.
     * \arg section specifies the section.
     * \arg defaultValue specifies the default value for the settings entry.
     * \arg description specifies a description for the settings entry.
     * \arg optionss specifies the options for the settings entry.
     */
    QgsSettingsEntryByReference( const QString &key,
                                 const QString &section,
                                 const T &defaultValue,
                                 const QString &description = QString(),
                                 Qgis::SettingsOptions options = Qgis::SettingsOptions() )
      : QgsSettingsEntryBase( key, section, defaultValue, description, options )
    {}


    virtual Qgis::SettingsType settingsType() const override = 0;

    /**
     * Returns settings value.
     *
     * \param dynamicKeyPart specifies the dynamic part of the settings key.
     */
    T value( const QString &dynamicKeyPart = QString() ) const { return this->convertFromVariant( valueAsVariant( dynamicKeyPart ) );}

    /**
     * Returns settings value.
     *
     * \param dynamicKeyPartList specifies the list of dynamic parts of the settings key.
     */
    T value( const QStringList &dynamicKeyPartList )  const { return this->convertFromVariant( valueAsVariant( dynamicKeyPartList ) );}


    //! Returns the settings value with a \a defaultValueOverride and with an optional \a dynamicKeyPart
    inline T valueWithDefaultOverride( const T &defaultValueOverride, const QString &dynamicKeyPart = QString() ) const
    {
      if ( this->exists( dynamicKeyPart ) )
        return this->value( dynamicKeyPart );
      else
        return defaultValueOverride;
    }

    //! Returns the settings value with a \a defaultValueOverride for the \a dynamicKeyPartList
    inline T valueWithDefaultOverride( const T &defaultValueOverride, const QStringList &dynamicKeyPartList ) const
    {
      if ( this->exists( dynamicKeyPartList ) )
        return this->value( dynamicKeyPartList );
      else
        return defaultValueOverride;
    }

    /**
     * Returns the settings value for the \a dynamicKeyPart and  with a \a defaultValueOverride
     * \deprecated since QGIS 3.26 use valueAsVariantWithDefaultOverride instead
     */
    Q_DECL_DEPRECATED T value( const QString &dynamicKeyPart, bool useDefaultValueOverride, const T &defaultValueOverride ) const SIP_DEPRECATED
    {
      if ( useDefaultValueOverride )
        return this->convertFromVariant( valueAsVariantWithDefaultOverride( defaultValueOverride, dynamicKeyPart ) );
      else
        return this->convertFromVariant( valueAsVariant( dynamicKeyPart ) );
    }

    /**
     * Returns the settings value for the \a dynamicKeyPartList and  with a \a defaultValueOverride
     * \deprecated since QGIS 3.26 use valueAsVariantWithDefaultOverride instead
     */
    Q_DECL_DEPRECATED T value( const QStringList &dynamicKeyPartList, bool useDefaultValueOverride, const T &defaultValueOverride ) const SIP_DEPRECATED
    {
      if ( useDefaultValueOverride )
        return this->convertFromVariant( valueAsVariantWithDefaultOverride( defaultValueOverride, dynamicKeyPartList ) );
      else
        return this->convertFromVariant( valueAsVariant( dynamicKeyPartList ) );
    }

    /**
     * Set settings value.
     *
     * The \a value to set.
     * \arg dynamicKeyPart specifies the dynamic part of the settings key.
     */
    bool setValue( const T &value, const QString &dynamicKeyPart = QString() ) const
    {
      return setValuePrivate( value, dynamicKeyPartToList( dynamicKeyPart ) );
    }

    /**
     * Set settings value.
     *
     * The \a value to set.
     * \arg dynamicKeyParts specifies the list of dynamic parts of the settings key.
     */
    bool setValue( const T &value, const QStringList &dynamicKeyPartList ) const
    {
      return setValuePrivate( value, dynamicKeyPartList );
    }

    //! Returns settings default value.
    T defaultValue() const {return convertFromVariant( defaultValueAsVariant() );}

    /**
     * Returns the former value.
     * Returns the current value (or default) if there is no former value.
     */
    T formerValue( const QString &dynamicKeyPart = QString() ) const {return convertFromVariant( formerValueAsVariant( dynamicKeyPart ) );}

    /**
     * Returns the former value
     * Returns the current value (or default) if there is no former value.
     */
    T formerValue( const QStringList &dynamicKeyPartList ) const {return convertFromVariant( formerValueAsVariant( dynamicKeyPartList ) );}

  protected:
    //! Sets the settings value with an optional list of dynamic parts
    bool setValuePrivate( const T &value, const QStringList &dynamicKeyPartList ) const
    {
      if ( checkValue( value ) )
        return QgsSettingsEntryBase::setVariantValuePrivate( convertToVariant( value ), dynamicKeyPartList );
      else
        return false;
    }

    //! Converts the variant value to the value type of the setting
    virtual T convertFromVariant( const QVariant &value ) const = 0;

    //! Converts the value to a variant
    virtual QVariant convertToVariant( const T &value ) const
    {
      return QVariant::fromValue( value );
    }

    //! Check if the value is valid
    virtual bool checkValue( const T &value ) const
    {
      Q_UNUSED( value )
      return true;
    }
};


/**
 * \ingroup core
 * \class QgsSettingsEntryByValue
 *
 * \brief Base abstract class for settings entry which are passed by value
 * \see QgsSettingsEntryBase
 * \see QgsSettingsEntryByReference
 *
 * \since QGIS 3.26
 */
template<class T>
class QgsSettingsEntryByValue : public QgsSettingsEntryBase
{
  public:

    /**
     * Constructor for QgsSettingsEntryByValue.
     *
     * \arg key specifies the key of the settings.
     * \arg parent specifies the parent in the tree of settings.
     * \arg section specifies the section.
     * \arg defaultValue specifies the default value for the settings entry.
     * \arg description specifies a description for the settings entry.
     * \arg optionss specifies the options for the settings entry.
     * \throws QgsSettingsException if the number of given parent named items doesn't match the complete key definition
     */
    QgsSettingsEntryByValue( const QString &key, QgsSettingsTreeNode *parent, QVariant defaultValue, const QString &description = QString(), Qgis::SettingsOptions options = Qgis::SettingsOptions() )
      : QgsSettingsEntryBase( key, parent, defaultValue, description, options )
    {}

    /**
     * Constructor for QgsSettingsEntryByValue.
     *
     * \arg key specifies the key of the settings.
     * \arg section specifies the section.
     * \arg defaultValue specifies the default value for the settings entry.
     * \arg description specifies a description for the settings entry.
     * \arg optionss specifies the options for the settings entry.
     */
    QgsSettingsEntryByValue( const QString &key, const QString &section, QVariant defaultValue, const QString &description = QString(), Qgis::SettingsOptions options = Qgis::SettingsOptions() )
      : QgsSettingsEntryBase( key, section, defaultValue, description, options )
    {}

    virtual Qgis::SettingsType settingsType() const override = 0;

    /**
     * Returns settings value.
     *
     * \arg dynamicKeyPart specifies the dynamic part of the settings key.
     * \arg defaultValueOverride if valid is used instead of the normal default value.
     */
    T value( const QString &dynamicKeyPart = QString() ) const { return this->convertFromVariant( valueAsVariant( dynamicKeyPart ) );}

    /**
     * Returns settings value.
     *
     * \arg dynamicKeyPartList specifies the list of dynamic parts of the settings key.
     * \arg defaultValueOverride if valid is used instead of the normal default value.
     */
    T value( const QStringList &dynamicKeyPartList )  const { return this->convertFromVariant( valueAsVariant( dynamicKeyPartList ) );}

    //! Returns the settings value with a \a defaultValueOverride and with an optional \a dynamicKeyPart
    inline T valueWithDefaultOverride( T defaultValueOverride, const QString &dynamicKeyPart = QString() ) const
    {
      if ( this->exists( dynamicKeyPart ) )
        return this->value( dynamicKeyPart );
      else
        return defaultValueOverride;
    }

    //! Returns the settings value with a \a defaultValueOverride for the \a dynamicKeyPartList
    inline T valueWithDefaultOverride( T defaultValueOverride, const QStringList &dynamicKeyPartList ) const
    {
      if ( this->exists( dynamicKeyPartList ) )
        return this->value( dynamicKeyPartList );
      else
        return defaultValueOverride;
    }

    /**
     * Returns the settings value for the \a dynamicKeyPart and  with a \a defaultValueOverride
     * \deprecated since QGIS 3.26 use valueWithDefaultOverride instead
     */
    Q_DECL_DEPRECATED T value( const QString &dynamicKeyPart, bool useDefaultValueOverride, T defaultValueOverride ) const SIP_DEPRECATED
    {
      if ( useDefaultValueOverride )
        return this->convertFromVariant( valueAsVariantWithDefaultOverride( defaultValueOverride, dynamicKeyPart ) );
      else
        return this->convertFromVariant( valueAsVariant( dynamicKeyPart ) );
    }

    /**
     * Returns the settings value for the \a dynamicKeyPartList and  with a \a defaultValueOverride
     * \deprecated since QGIS 3.26 use valueWithDefaultOverride instead
     */
    Q_DECL_DEPRECATED T value( const QStringList &dynamicKeyPartList, bool useDefaultValueOverride, T defaultValueOverride ) const  SIP_DEPRECATED
    {
      if ( useDefaultValueOverride )
        return this->convertFromVariant( valueAsVariantWithDefaultOverride( defaultValueOverride, dynamicKeyPartList ) );
      else
        return this->convertFromVariant( valueAsVariant( dynamicKeyPartList ) );
    }

    /**
     * Set settings value.
     *
     * The \a value to set.
     * \arg dynamicKeyPart specifies the dynamic part of the settings key.
     */
    bool setValue( T value, const QString &dynamicKeyPart = QString() ) const
    {
      return setValuePrivate( value, dynamicKeyPartToList( dynamicKeyPart ) );
    }

    /**
     * Set settings value.
     *
     * The \a value to set.
     * \arg dynamicKeyParts specifies the list of dynamic parts of the settings key.
     */
    bool setValue( T value, const QStringList &dynamicKeyPartList ) const
    {
      return setValuePrivate( value, dynamicKeyPartList );
    }

    //! Returns settings default value.
    T defaultValue() const {return convertFromVariant( defaultValueAsVariant() );}

    /**
     * Returns the former value
     * Returns the current value (or default) if there is no former value.
     */
    T formerValue( const QString &dynamicKeyPart = QString() ) const {return convertFromVariant( formerValueAsVariant( dynamicKeyPart ) );}

    /**
     * Returns the former value
     * Returns the current value (or default) if there is no former value.
     */
    T formerValue( const QStringList &dynamicKeyPartList ) const {return convertFromVariant( formerValueAsVariant( dynamicKeyPartList ) );}

  protected:
    //! Sets the settings value with an optional list of dynamic parts
    virtual bool setValuePrivate( T value, const QStringList &dynamicKeyPartList ) const
    {
      if ( checkValue( value ) )
        return QgsSettingsEntryBase::setVariantValuePrivate( convertToVariant( value ), dynamicKeyPartList );
      else
        return false;
    }

    //! Converts the variant value to the value type of the setting
    virtual T convertFromVariant( const QVariant &value ) const = 0;

    //! Converts the value to a variant
    virtual QVariant convertToVariant( T value ) const
    {
      return QVariant::fromValue( value );
    }

    //! Check if the value is valid
    virtual bool checkValue( T value ) const
    {
      Q_UNUSED( value )
      return true;
    }
};


#endif // QGSSETTINGSENTRY_H
