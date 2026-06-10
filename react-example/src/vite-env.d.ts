/// <reference types="vite/client" />
import React from 'react'

declare global {
  namespace JSX {
    interface IntrinsicElements {
      'samkoon-monitor-sdk': React.DetailedHTMLProps<
        React.HTMLAttributes<HTMLElement> & {
          'instance-id'?: string
          'client-id'?: string
          'secret-key'?: string
        },
        HTMLElement
      >
    }
  }
}
