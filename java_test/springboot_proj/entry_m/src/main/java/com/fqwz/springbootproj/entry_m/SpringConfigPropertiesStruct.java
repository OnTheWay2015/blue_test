package com.fqwz.springbootproj.entry_m;


import org.springframework.boot.context.properties.ConfigurationProperties;
import org.springframework.stereotype.Component;

@Component
@ConfigurationProperties(prefix = "spring")
public class SpringConfigPropertiesStruct {

    private boolean debug;

    // Getter and Setter
    public boolean getProperty() {
        return debug;
    }

    public void setProperty(boolean debug) {
        this.debug = debug;
    }
}
